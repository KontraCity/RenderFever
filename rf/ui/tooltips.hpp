#pragma once

#include <rf/auxiliary/fs.hpp>
#include <rf/auxiliary/imgui.hpp>

#include <rf/core/engine.hpp>
#include <rf/resources/library.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

namespace Ui {
    namespace Tooltips {
        constexpr float TooltipWidth = 200.0f;

        inline void DrawUnloadedResourceTooltip(const fs::path& path, const char* resourceName) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;

            ImGui::BeginTooltip();
            ImUtil::TextShifted(ImUtil::TruncateText(
                path.filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::Text("Unloaded %s", resourceName);
            ImGui::EndTooltip();
        }

        inline void DrawShaderTooltip(const Resources::Shader& shader) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;

            const IconMap& iconMap = Engine::Overlay().iconMap();
            PreviewMap& previewMap = Engine::Overlay().previewMap();

            ImGui::BeginTooltip();
            ImGui::Image(previewMap.getShaderPreview(shader).handle(), ImVec2(TooltipWidth, TooltipWidth));
            ImGui::Image(iconMap.regularFileVert().handle(), ImVec2(TooltipWidth / 3, TooltipWidth / 3));
            ImGui::SameLine(0, 0);
            ImGui::BeginDisabled(!shader->geometryStage());
            ImGui::Image(iconMap.regularFileGeom().handle(), ImVec2(TooltipWidth / 3, TooltipWidth / 3));
            ImGui::EndDisabled();
            ImGui::SameLine(0, 0);
            ImGui::Image(iconMap.regularFileFrag().handle(), ImVec2(TooltipWidth / 3, TooltipWidth / 3));
            ImUtil::TextShifted(ImUtil::TruncateText(
                shader.path().filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::Text("Shader resource #%d", shader.id());
            ImGui::TextUnformatted(shader->geometryStage() ? "3 stages" : "2 stages");
            ImGui::EndTooltip();
        }

        inline void DrawTextureTooltip(const Resources::Texture& texture) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;

            ImGui::BeginTooltip();
            ImGui::Image(texture->handle(), ImVec2(TooltipWidth, TooltipWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                texture.path().filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::Text("Texture resource #%d", texture.id());
            ImGui::Text(
                "%dx%d pixels",
                texture->dimensions().width,
                texture->dimensions().height
            );
            ImGui::EndTooltip();
        }
    
        inline void DrawMeshTooltip(const Resources::Mesh& mesh) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;
            const Graphics::Texture& meshPreview = Engine::Overlay().previewMap().getMeshPreview(mesh);

            ImGui::BeginTooltip();
            ImGui::Image(meshPreview.handle(), ImVec2(TooltipWidth, TooltipWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                mesh.path().filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::Text("Mesh resource #%d", mesh.id());
            ImGui::Text(
                "%s %s, %s %s",
                ImUtil::CompactNumber(mesh->trianglesCount()).c_str(),
                ImUtil::PluralizeIfNeeded("triangle", mesh->trianglesCount()).c_str(),
                ImUtil::CompactNumber(mesh->verticesCount()).c_str(),
                ImUtil::PluralizeIfNeeded("vertice", mesh->verticesCount()).c_str()
            );
            ImGui::EndTooltip();
        }

        inline void DrawModelTooltip(const Resources::Model& model) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;
            const Graphics::Texture& modelPreview = Engine::Overlay().previewMap().getModelPreview(model);

            ImGui::BeginTooltip();
            ImGui::Image(modelPreview.handle(), ImVec2(TooltipWidth, TooltipWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                model.path().filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::Text("Model resource #%d", model.id());
            ImGui::Text(
                "%s %s",
                ImUtil::CompactNumber(model->meshes().size()).c_str(),
                ImUtil::PluralizeIfNeeded("mesh", model->meshes().size(), "es").c_str()
            );
            ImGui::Text(
                "%s %s, %s %s",
                ImUtil::CompactNumber(model->trianglesCount()).c_str(),
                ImUtil::PluralizeIfNeeded("triangle", model->trianglesCount()).c_str(),
                ImUtil::CompactNumber(model->verticesCount()).c_str(),
                ImUtil::PluralizeIfNeeded("vertice", model->verticesCount()).c_str()
            );
            ImGui::EndTooltip();
        }
    }
}


} // namespace rf
