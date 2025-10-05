#include "renderer.hpp"

#include <utility>

#include <rf/core/engine.hpp>
#include <rf/core/math.hpp>
#include <rf/graphics/lighting.hpp>
#include <rf/graphics/shader.hpp>
#include <rf/world/components.hpp>

namespace rf {

static void SetStorageSize(Graphics::Storage& storage, GLint size) {
    storage.write(&size, sizeof(size));
}

Graphics::Renderer::Renderer(const Config& config, const Resources::Library& library) 
    : m_lightStorage(Storage::Type::Lighting, sizeof(GLint) * 4 + sizeof(Light) * config.lightSourcesReserve)
    , m_depthTestingMode(config.depthTestingMode) 
    , m_faceCullingMode(config.faceCullingMode) 
    , m_wireframeMode(config.wireframeMode) {
    m_shaders.main = library.loadShader(config.mainShaderPath);
    m_shaders.light = library.loadShader(config.lightShaderPath);
    SetStorageSize(m_lightStorage, 0);

    setDepthTestingMode(m_depthTestingMode);
    setFaceCullingMode(m_faceCullingMode);
    setWireframeMode(m_wireframeMode);
}

Graphics::Renderer::Renderer(Renderer&& other) noexcept
    : m_shaders(std::exchange(other.m_shaders, {}))
    , m_lightStorage(std::move(other.m_lightStorage))
    , m_depthTestingMode(std::exchange(other.m_depthTestingMode, true))
    , m_faceCullingMode(std::exchange(other.m_faceCullingMode, true))
    , m_wireframeMode(std::exchange(other.m_wireframeMode, false))
{}

Graphics::Renderer& Graphics::Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        m_shaders = std::exchange(other.m_shaders, {});
        m_lightStorage = std::move(other.m_lightStorage);
        m_depthTestingMode = std::exchange(other.m_depthTestingMode, true);
        m_faceCullingMode = std::exchange(other.m_faceCullingMode, true);
        m_wireframeMode = std::exchange(other.m_wireframeMode, false);
    }
    return *this;
}

void Graphics::Renderer::clear() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::Renderer::capture() {
    const Camera& camera = *Engine::Scene().get<Camera>().get();
    m_shaders.main->capture(camera);
    m_shaders.light->capture(camera);
}

void Graphics::Renderer::illuminate() {
    glm::mat4 view = Math::EvaluateView(*Engine::Scene().get<Camera>().get());
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
            offset += m_lightStorage.write(lights.data(), iterator.count() * sizeof(Light), offset);
        }
        SetStorageSize(m_lightStorage, totalLights);
    });
}

void Graphics::Renderer::draw() {
    Engine::Scene().each([this](const World::DrawComponent& draw) {
        const Shader* shader = draw.shader.get();
        shader->transform(draw.transform);
        shader->material(draw.material);
        shader->draw(draw.mesh);
    });
}

void Graphics::Renderer::render() {
    clear();
    capture();
    illuminate();
    draw();
}

} // namespace rf
