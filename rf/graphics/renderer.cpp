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

Graphics::Renderer::Renderer(const Config& config, Resources::Library& library) 
    : m_lightStorage(Storage::Type::Lighting, sizeof(GLint) * 4 + sizeof(Light) * config.lightSourcesReserve)
    , m_previewFramebuffer(config.previewFramebufferDimensions)
    , m_depthTestingMode(config.depthTestingMode) 
    , m_faceCullingMode(config.faceCullingMode) 
    , m_wireframeMode(config.wireframeMode) {
    m_mainShader = library.loadShader(config.mainShaderPath);
    m_lightShader = library.loadShader(config.lightShaderPath);
    SetStorageSize(m_lightStorage, 0);

    setDepthTestingMode(m_depthTestingMode);
    setFaceCullingMode(m_faceCullingMode);
    setWireframeMode(m_wireframeMode);
}

Graphics::Renderer::Renderer(Renderer&& other) noexcept
    : m_mainShader(std::move(other.m_mainShader))
    , m_lightShader(std::move(other.m_lightShader))
    , m_lightStorage(std::move(other.m_lightStorage))
    , m_previewFramebuffer(std::move(other.m_previewFramebuffer))
    , m_depthTestingMode(std::exchange(other.m_depthTestingMode, true))
    , m_faceCullingMode(std::exchange(other.m_faceCullingMode, true))
    , m_wireframeMode(std::exchange(other.m_wireframeMode, false))
{}

Graphics::Renderer& Graphics::Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        m_mainShader = std::move(other.m_mainShader);
        m_lightShader = std::move(other.m_lightShader);
        m_lightStorage = std::move(other.m_lightStorage);
        m_previewFramebuffer = std::move(other.m_previewFramebuffer);
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

void Graphics::Renderer::capture(const World::Scene& scene) {
    const Camera* camera = scene.getActiveCamera();
    if (!camera)
        return;

    m_mainShader->capture(*camera);
    m_lightShader->capture(*camera);
}

void Graphics::Renderer::illuminate(const World::Scene& scene) {
    const Camera* camera = scene.getActiveCamera();
    if (!camera)
        return;

    glm::mat4 view = Math::EvaluateView(*camera);
    scene.query<World::LightComponent>().run([this, &view](flecs::iter& iterator) {
        GLint totalLights = 0;
        size_t offset = sizeof(totalLights) * 4;
        while (iterator.next()) {
            const World::LightComponent* buffer = &iterator.field<World::LightComponent>(0)[0];
            std::vector<Light> lights(iterator.count());
            for (size_t index = 0, size = iterator.count(); index < size; ++index)
                lights[index] = (buffer + index)->light;

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

void Graphics::Renderer::draw(const World::Scene& scene) {
    scene.each([this](const World::DrawComponent& draw) {
        const Shader* shader = draw.material.shader.get();
        shader->transform(draw.transform);
        shader->material(draw.material);
        shader->draw(*draw.mesh);
    });
}

void Graphics::Renderer::render(const World::Scene* previewScene) {
    // TODO: Return bind handles.
    if (previewScene)
        m_previewFramebuffer.bind();

    const World::Scene& scene = previewScene ? *previewScene : Engine::Scene();
    clear();
    capture(scene);
    illuminate(scene);
    draw(scene);

    if (previewScene)
        m_previewFramebuffer.unbind();
}

} // namespace rf
