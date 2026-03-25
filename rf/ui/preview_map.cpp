#include "preview_map.hpp"

#include <rf/auxiliary/imgui.hpp>

#include <rf/core/engine.hpp>
#include <rf/core/math.hpp>
#include <rf/ui/embedded/container_textures_png.hpp>
#include <rf/ui/embedded/cube_mesh_glb.hpp>
#include <rf/world/scene.hpp>

namespace rf {

Ui::PreviewMap::PreviewMap()
    : m_cubeMesh(std::make_shared<Graphics::Mesh>(Resources::Library::LoadMesh(Embedded::CubeMeshGlb(), "glb")))
    , m_containerDiffuseTexture(std::make_shared<Graphics::Texture>(Image(Embedded::ContainerDiffuseTexturePng())))
    , m_containerSpecularTexture(std::make_shared<Graphics::Texture>(Image(Embedded::ContainerSpecularTexturePng())))
{}

Graphics::Texture Ui::PreviewMap::renderShaderPreview(const Resources::Shader& shader) {
    static World::Scene s_previewScene;
    static World::Entity* s_previewEntity = nullptr;
    if (!s_previewEntity) {
        World::Entity& observer = s_previewScene.newEntity("Observer");
        observer.add<World::CameraComponent>();
        s_previewScene.setActiveCameraEntity(observer);
        Math::DirectCameraAtSphere(*s_previewScene.getActiveCamera(), m_cubeMesh->center(), m_cubeMesh->radius());

        World::Entity& light = s_previewScene.newEntity("Light");
        light.set<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::DirectionalLight,
                .direction = { 0.577, -0.577, 0.577 },
            }
        });

        World::Entity& object = s_previewScene.newEntity("Object");
        object.add<rf::World::TransformComponent>();
        object.set<World::MeshDrawComponent>({
            .material = {
                .diffuse = Resources::Texture(m_containerDiffuseTexture),
                .specular = Resources::Texture(m_containerSpecularTexture),
                .shininess = 32.0f,
            },
            .mesh = Resources::Mesh(m_cubeMesh)
        });
        s_previewEntity = &object;
    }
    s_previewEntity->get<World::MeshDrawComponent>()->material.shader = shader;

    Graphics::Renderer& renderer = Engine::Renderer();
    renderer.render(&s_previewScene);
    return renderer.previewFramebuffer().texture();
}

Graphics::Texture Ui::PreviewMap::renderMeshPreview(const Resources::Mesh& mesh) {
    static World::Scene s_previewScene;
    static World::Entity* s_previewEntity = nullptr;
    if (!s_previewEntity) {
        World::Entity& observer = s_previewScene.newEntity("Observer");
        observer.add<World::CameraComponent>();
        s_previewScene.setActiveCameraEntity(observer);

        World::Entity& light = s_previewScene.newEntity("Light");
        light.set<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::DirectionalLight,
                .direction = { -0.276f, -0.920f, -0.276f },
            }
        });

        World::Entity& object = s_previewScene.newEntity("Object");
        object.add<rf::World::TransformComponent>();
        object.set<World::MeshDrawComponent>({
            .material = {
                .shader = Engine::Renderer().mainShader()
            }
        });
        s_previewEntity = &object;
    }
    Math::DirectCameraAtSphere(*s_previewScene.getActiveCamera(), mesh->center(), mesh->radius());
    s_previewEntity->get<World::MeshDrawComponent>()->mesh = mesh;

    Graphics::Renderer& renderer = Engine::Renderer();
    renderer.render(&s_previewScene);
    return renderer.previewFramebuffer().texture();
}

Graphics::Texture Ui::PreviewMap::renderModelPreview(const Resources::Model& model) {
    static World::Scene s_previewScene;
    static World::Entity* s_previewEntity = nullptr;
    if (!s_previewEntity) {
        World::Entity& observer = s_previewScene.newEntity("Observer");
        observer.add<World::CameraComponent>();
        s_previewScene.setActiveCameraEntity(observer);

        World::Entity& light = s_previewScene.newEntity("Light");
        light.set<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::DirectionalLight,
                .direction = { -0.276f, -0.920f, -0.276f },
            }
        });

        World::Entity& object = s_previewScene.newEntity("Object");
        object.add<rf::World::TransformComponent>();
        object.add<World::ModelDrawComponent>();
        s_previewEntity = &object;
    }
    Math::DirectCameraAtSphere(*s_previewScene.getActiveCamera(), model->center(), model->radius());
    s_previewEntity->get<World::ModelDrawComponent>()->model = model;

    Graphics::Renderer& renderer = Engine::Renderer();
    renderer.render(&s_previewScene);
    return renderer.previewFramebuffer().texture();
}

bool Ui::PreviewMap::containsShaderPreview(const Resources::Shader& shader) const {
    return m_shaderPreviews.find(shader.id()) != m_shaderPreviews.end();
}

bool Ui::PreviewMap::containsMeshPreview(const Resources::Mesh& mesh) const {
    return m_meshPreviews.find(mesh.id()) != m_meshPreviews.end();
}

bool Ui::PreviewMap::containsModelPreview(const Resources::Model& model) const {
    return m_modelPreviews.find(model.id()) != m_modelPreviews.end();
}

const Graphics::Texture& Ui::PreviewMap::getShaderPreview(const Resources::Shader& shader) {
    auto entry = m_shaderPreviews.find(shader.id());
    if (entry != m_shaderPreviews.end())
        return entry->second;
    return m_shaderPreviews.emplace(shader.id(), renderShaderPreview(shader)).first->second;
}

const Graphics::Texture& Ui::PreviewMap::getMeshPreview(const Resources::Mesh& mesh) {
    auto entry = m_meshPreviews.find(mesh.id());
    if (entry != m_meshPreviews.end())
        return entry->second;
    return m_meshPreviews.emplace(mesh.id(), renderMeshPreview(mesh)).first->second;
}

const Graphics::Texture& Ui::PreviewMap::getModelPreview(const Resources::Model& model) {
    auto entry = m_modelPreviews.find(model.id());
    if (entry != m_modelPreviews.end())
        return entry->second;
    return m_modelPreviews.emplace(model.id(), renderModelPreview(model)).first->second;
}

void Ui::PreviewMap::resetShaderPreview(const Resources::Shader& shader) {
    auto entry = m_shaderPreviews.find(shader.id());
    if (entry == m_shaderPreviews.end())
        return;
    entry->second = std::move(renderShaderPreview(shader));
}

void Ui::PreviewMap::resetMeshPreview(const Resources::Mesh& mesh) {
    auto entry = m_meshPreviews.find(mesh.id());
    if (entry == m_meshPreviews.end())
        return;
    entry->second = std::move(renderMeshPreview(mesh));
}

void Ui::PreviewMap::resetModelPreview(const Resources::Model& model) {
    auto entry = m_modelPreviews.find(model.id());
    if (entry == m_modelPreviews.end())
        return;
    entry->second = std::move(renderModelPreview(model));
}

} // namespace rf
