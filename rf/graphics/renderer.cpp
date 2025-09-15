#include "renderer.hpp"

#include <fmt/format.h>

#include "rf/core/engine.hpp"
#include "rf/core/stopwatch.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/transform.hpp"
#include "rf/world/entity.hpp"

namespace rf {

static void SetStorageSize(Storage& storage, GLint size) {
    storage.write(&size, sizeof(size));
}

void Renderer::clear() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::capture() {
    const Camera& camera = *Engine::Scene().get<Camera>().get();
    m_mainShader->capture(camera);
    m_lightShader->capture(camera);
}

void Renderer::illuminate() {
    glm::mat4 view = Engine::Scene().get<Camera>().get()->evaluateView();
    Engine::Scene().query<Light>().run([this, &view](flecs::iter& iterator) {
        GLint totalLights = 0;
        size_t offset = sizeof(totalLights) * 4;
        while (iterator.next()) {
            const Light* buffer = &iterator.field<Light>(0)[0];
            std::vector<Light> lights(buffer, buffer + iterator.count());

            for (Light& light : lights) {
                switch (light.type) {
                    case LightType::DirectionalLight:
                        light.direction = glm::vec3(view * glm::vec4(light.direction, 0.0f));
                        break;
                    case LightType::PointLight:
                        light.position = glm::vec3(view * glm::vec4(light.position, 1.0f));
                        break;
                    case LightType::SpotLight:
                        light.position = glm::vec3(view * glm::vec4(light.position, 1.0f));
                        light.direction = glm::vec3(view * glm::vec4(light.direction, 0.0f));
                        light.spotInnerCutoff = glm::cos(glm::radians(light.spotInnerCutoff));
                        light.spotOuterCutoff = glm::cos(glm::radians(light.spotOuterCutoff));
                        break;
                }
            }

            totalLights += iterator.count();
            offset += m_lightStorage->write(lights.data(), iterator.count() * sizeof(Light), offset);
        }
        SetStorageSize(*m_lightStorage, totalLights);
    });
}

void Renderer::draw() {
    Engine::Scene().each([this](const DrawComponent& draw) {
        Shader& shader = draw.shaderType == Shader::Type::Main ? *m_mainShader : *m_lightShader;
        shader.transform(draw.transform);
        shader.material(draw.material);
        shader.draw(draw.mesh);
    });
}

void Renderer::init(const Config& config) {
    if (m_init)
        return;
    
    Stopwatch stopwatch;
    m_mainShader = std::make_unique<Shader>(config.mainShaderConfig);
    m_lightShader = std::make_unique<Shader>(config.lightShaderConfig);
    m_lightStorage = std::make_unique<Storage>(0, sizeof(GLint) * 4 + sizeof(Light) * config.lightSourcesReserve);
    SetStorageSize(*m_lightStorage, 0);
    fmt::print("Renderer initialized in {} ms\n", stopwatch.milliseconds());

    m_init = true;
}

void Renderer::render() {
    if (!m_init) {
        fmt::print("Renderer is not initialized to render!\n");
        return;
    }

    clear();
    capture();
    illuminate();
    draw();
}

} // namespace rf
