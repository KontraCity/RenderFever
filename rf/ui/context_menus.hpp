#pragma once

#include <rf/auxiliary/imgui.hpp>

#include <rf/core/engine.hpp>
#include <rf/resources/library.hpp>

namespace rf {

namespace Ui {
    namespace ContextMenus {
        inline void DrawShaderContextMenu(Resources::Shader& shader, bool removable, const fs::path& resourcePath = {}) {
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem(shader ? "Reload" : "Load")) {
                    if (shader) {
                        Engine::Library().reloadShader(shader);
                        Engine::Overlay().previewMap().resetShaderPreview(shader);
                    }
                    else {
                        Engine::Library().loadShader(resourcePath);
                    }
                }
                else if (removable && ImGui::MenuItem("Remove")) {
                    shader = {};
                }
                ImGui::EndPopup();
            }
        }

        inline void DrawTextureContextMenu(Resources::Texture& texture, bool removable, const fs::path& resourcePath = {}) {
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem(texture ? "Reload" : "Load")) {
                    if (texture)
                        Engine::Library().reloadTexture(texture);
                    else
                        Engine::Library().loadTexture(resourcePath);
                }
                else if (removable && ImGui::MenuItem("Remove")) {
                    texture = {};
                }
                ImGui::EndPopup();
            }
        }

        inline void DrawMeshContextMenu(Resources::Mesh& mesh, bool removable, const fs::path& resourcePath = {}) {
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem(mesh ? "Reload" : "Load")) {
                    if (mesh) {
                        Engine::Library().reloadMesh(mesh);
                        Engine::Overlay().previewMap().resetMeshPreview(mesh);
                    }
                    else {
                        Engine::Library().loadMesh(resourcePath);
                    }
                }
                else if (removable && ImGui::MenuItem("Remove")) {
                    mesh = {};
                }
                ImGui::EndPopup();
            }
        }
    }
}

} // namespace rf
