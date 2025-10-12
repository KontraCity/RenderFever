#include "scene.hpp"

#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/core/math.hpp>
#include <rf/ui/imutil.hpp>
#include <rf/world/scene.hpp>

namespace rf {

static ImTextureID RenderMeshPreviewScene(const Resources::Mesh& mesh) {
    static World::Scene s_previewScene;
    static World::Entity* s_previewEntity = nullptr;
    static bool s_configured = false;
    if (!s_configured) {
        World::Entity& entity = s_previewScene.newEntity();
        entity.setComponent<World::DrawComponent>({
            .material = { .shader = Engine::Library().loadShader("main/") },
        });
        entity.setComponent<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::DirectionalLight,
                .direction = { -0.3f, -1.0f, -0.3f },
            }
        });

        Math::DirectCameraAtMesh(*s_previewScene.get<rf::Graphics::Camera>().get(), *mesh);
        s_previewEntity = &entity;
        s_configured = true;
    }
    s_previewEntity->getComponent<World::DrawComponent>()->mesh = mesh;

    Graphics::Renderer& renderer = Engine::Renderer();
    renderer.render(&s_previewScene);
    return static_cast<ImTextureID>(renderer.previewFramebuffer().texture());
}

static void Render(const char* id, const Resources::Shader& shader) {
    if (!shader.isValid()) {
        ImGui::BeginDisabled();
        ImGui::Button(fmt::format("[None]##{}", id).c_str(), ImVec2(-FLT_MIN, 0));
        ImGui::EndDisabled();
        return;
    }

    ImGui::Button(fmt::format("Shader resource #{}##{}", shader.id(), id).c_str(), ImVec2(-FLT_MIN, 0));
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();

        ImGui::Button("Vertex", ImVec2(75, 75));
        ImGui::SameLine();
        if (!shader.info().geometryStage)
            ImGui::BeginDisabled();
        ImGui::Button("Geometry", ImVec2(75, 75));
        if (!shader.info().geometryStage)
            ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::Button("Fragment", ImVec2(75, 75));

        ImGui::TextUnformatted(fmt::format("Shader resource #{}", shader.id()).c_str());
        ImGui::TextUnformatted(fmt::format("\"{}\"", shader.info().vertexFilePath.string()).c_str());
        if (shader.info().geometryStage)
            ImGui::TextUnformatted(fmt::format("\"{}\"", shader.info().geometryFilePath.string()).c_str());
        ImGui::TextUnformatted(fmt::format("\"{}\"", shader.info().fragmentFilePath.string()).c_str());

        ImGui::EndTooltip();
    }
}

static void Render(const char* id, const Resources::Texture& texture) {
    if (!texture.isValid()) {
        ImGui::BeginDisabled();
        ImGui::Button(fmt::format("[None]##{}", id).c_str(), ImVec2(-FLT_MIN, 0));
        ImGui::EndDisabled();
        return;
    }

    ImGui::Button(fmt::format("Texture resource #{}##{}", texture.id(), id).c_str(), ImVec2(-FLT_MIN, 0));
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Image(static_cast<ImTextureID>(texture->handle()), ImVec2(100, 100));
        ImGui::TextUnformatted(fmt::format("Texture resource #{}", texture.id()).c_str());
        ImGui::TextUnformatted(fmt::format(
            "{}x{}, {} channel{}",
            texture.info().dimensions.width,
            texture.info().dimensions.height,
            texture.info().dimensions.channels,
            (texture.info().dimensions.channels == 1) ? "" : "s"
        ).c_str());
        ImGui::TextUnformatted(fmt::format("\"{}\"", texture.path().string()).c_str());
        ImGui::EndTooltip();
    }
}

static void Render(const char* id, const Resources::Mesh& mesh) {
    if (!mesh.isValid()) {
        ImGui::BeginDisabled();
        ImGui::Button(fmt::format("[None]##{}", id).c_str(), ImVec2(-FLT_MIN, 0));
        ImGui::EndDisabled();
        return;
    }

    ImGui::Button(fmt::format("Mesh resource #{}##{}", mesh.id(), id).c_str(), ImVec2(-FLT_MIN, 0));
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::Image(RenderMeshPreviewScene(mesh), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::TextUnformatted(fmt::format("Mesh resource #{}", mesh.id()).c_str());
        ImGui::TextUnformatted(fmt::format(
            "{} indice{}, {} triangle{}",
            mesh->indicesCount(),
            mesh->indicesCount() == 1 ? "" : "s",
            mesh->indicesCount() / 3,
            mesh->indicesCount() / 3 == 1 ? "" : "s"
        ).c_str());
        ImGui::TextUnformatted(fmt::format("\"{}\"", mesh.path().string()).c_str());
        ImGui::EndTooltip();
    }
}

static void Render(World::DrawComponent* component) {
    if (!ImGui::CollapsingHeader("Draw Component"))
        return;

    if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("##scene_draw_component_transform_table", 2)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Position");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::DragFloat3("##scene_draw_component_transform_position", glm::value_ptr(component->transform.position), 0.01f);
            ImGui::PopItemWidth();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Rotation");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::DragFloat3("##scene_draw_component_transform_rotation", glm::value_ptr(component->transform.rotation), 0.5f);
            ImGui::PopItemWidth();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Scale");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::DragFloat3("##scene_draw_component_transform_scale", glm::value_ptr(component->transform.scale), 0.01f);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
    
    if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("##scene_draw_component_material_table", 2)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Shader");
            ImGui::TableNextColumn();
            Render("##scene_draw_component_material_shader", component->material.shader);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Diffuse texture");
            ImGui::TableNextColumn();
            Render("##scene_draw_component_material_diffuse", component->material.diffuse);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Specular texture");
            ImGui::TableNextColumn();
            Render("##scene_material_draw_component_specular", component->material.specular);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Shininess");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::SliderFloat("##scene_draw_component_material_shininess", &component->material.shininess, 1.0f, 256.0f, "%.0f", ImGuiSliderFlags_Logarithmic);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("##scene_draw_component_material_table", 2)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Mesh");
            ImGui::TableNextColumn();
            Render("##scene_draw_component_mesh", component->mesh);

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
}

static void Render(World::LightComponent* component) {
    if (!ImGui::CollapsingHeader("Light Component"))
        return;
    Graphics::Light& light = component->light;

    if (ImGui::TreeNodeEx("Common", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("##scene_light_component_common_table", 2)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Type");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            if (ImGui::BeginCombo("##scene_light_component_light_type", Graphics::LightTypeToString(light.type))) {
                if (ImGui::Selectable(Graphics::LightTypeToString(Graphics::LightType::DirectionalLight), light.type == Graphics::LightType::DirectionalLight))
                    light.type = Graphics::LightType::DirectionalLight;
                else if (ImGui::Selectable(Graphics::LightTypeToString(Graphics::LightType::PointLight), light.type == Graphics::LightType::PointLight))
                    light.type = Graphics::LightType::PointLight;
                else if (ImGui::Selectable(Graphics::LightTypeToString(Graphics::LightType::SpotLight), light.type == Graphics::LightType::SpotLight))
                    light.type = Graphics::LightType::SpotLight;
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            if (light.type == Graphics::LightType::PointLight || light.type == Graphics::LightType::SpotLight) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Position");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::DragFloat3("##scene_light_component_light_position", glm::value_ptr(light.position), 0.01f);
                ImGui::PopItemWidth();
            }

            if (light.type == Graphics::LightType::DirectionalLight || light.type == Graphics::LightType::SpotLight) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Direction");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::DragFloat3("##scene_light_component_light_direction", glm::value_ptr(light.direction), 0.01f);
                ImGui::PopItemWidth();
            }

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Color");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::ColorEdit3("##scene_light_component_light_color", glm::value_ptr(light.color));
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("##scene_light_component_properties_table", 2)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Ambient");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::SliderFloat("##scene_light_component_light_ambient_property", &light.ambientProperty, 0.0f, 1.0f);
            ImGui::PopItemWidth();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Diffuse");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::SliderFloat("##scene_light_component_light_diffuse_property", &light.diffuseProperty, 0.0f, 1.5f);
            ImGui::PopItemWidth();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Specular");
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::SliderFloat("##scene_light_component_light_specular_property", &light.specularProperty, 0.0f, 2.0f);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
        ImGui::TreePop();
    }
   
    if (light.type == Graphics::LightType::PointLight || light.type == Graphics::LightType::SpotLight) {
        if (ImGui::TreeNodeEx("Attenuation", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable("##scene_light_component_attenuation_table", 2)) {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Constant");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderFloat("##scene_light_component_light_constant_attenuation", &light.constantAttenuation, 0.0f, 2.0f);
                ImGui::PopItemWidth();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Linear");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderFloat("##scene_light_component_light_linear_attenuation", &light.linearAttenuation, 0.0f, 1.0f);
                ImGui::PopItemWidth();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Quadratic");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderFloat("##scene_light_component_light_quadratic_attenuation", &light.quadraticAttenuation, 0.0f, 0.2f);
                ImGui::PopItemWidth();

                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }

    if (light.type == Graphics::LightType::SpotLight) {
        if (ImGui::TreeNodeEx("Spot cutoff", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable("##scene_light_component_spot_cutoff_table", 2)) {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Inner cutoff");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderFloat("##scene_light_component_light_inner_cutoff", &light.spotInnerCutoff, 0.0f, 90.0f);
                ImGui::PopItemWidth();
                bool innerCutoffSliderActive = ImGui::IsItemActive();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("Outer cutoff");
                ImGui::TableNextColumn();
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::SliderFloat("##scene_light_component_light_outer_cutoff", &light.spotOuterCutoff, 0.0f, 90.0f);
                ImGui::PopItemWidth();
                bool outerCutoffSliderActive = ImGui::IsItemActive();

                if (light.spotInnerCutoff > light.spotOuterCutoff) {
                    if (innerCutoffSliderActive)
                        light.spotOuterCutoff = light.spotInnerCutoff;
                    else if (outerCutoffSliderActive)
                        light.spotInnerCutoff = light.spotOuterCutoff;
                }

                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }
}

void Ui::Windows::Scene::update() {
    World::Scene& scene = Engine::Scene();
    World::Scene::Entities& entities = scene.entities();
    static World::EntityId s_selectedEntityId = 0;

    ImGui::BeginChild("##scene_left_pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    {
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            s_selectedEntityId = 0;

        for (const auto& entityEntry : entities) {
            const World::Entity& entity = entityEntry.second;
            if (ImGui::Selectable(entity.name(), entity.id() == s_selectedEntityId))
                s_selectedEntityId = entity.id();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##scene_item_view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    {
        auto entityEntry = entities.find(s_selectedEntityId);
        if (entityEntry == entities.end()) {
            ImGui::TextUnformatted("No entity selected");
            ImGui::Separator();
            ImUtil::TextShifted(fmt::format(
                "{} entitie{} spawned",
                entities.size(),
                entities.size() == 1 ? "" : "s"
            ).c_str(), 0.5f, 0.5f);
        }
        else {
            World::Entity& entity = entityEntry->second;
            ImGui::TextUnformatted(fmt::format("Entity: {}", entity.name()).c_str());
            ImGui::Separator();
            if (ImGui::BeginTabBar(fmt::format("##scene_item_#{}_tabs", entity.id()).c_str(), ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Info")) {
                    if (ImGui::BeginTable("##scene_item_table", 2)) {
                        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1.0f);
                        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted("Entity ID");
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(fmt::format("#{}", entity.id()).c_str());

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted("Name");
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(entity.name());

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Components")) {
                    entity.each([&scene, &entity](flecs::id id) {
                        if (scene.is<World::DrawComponent>(id))
                            Render(entity.getComponent<World::DrawComponent>());
                        else if (scene.is<World::LightComponent>(id))
                            Render(entity.getComponent<World::LightComponent>());
                    });
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
    }
    ImGui::EndChild();
}

} // namespace rf
