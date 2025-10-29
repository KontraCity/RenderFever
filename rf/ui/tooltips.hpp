#pragma once

#include <fmt/format.h>

#include <rf/auxiliary/fs.hpp>
#include <rf/auxiliary/imgui.hpp>

#include <rf/core/engine.hpp>
#include <rf/resources/library.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

namespace Ui {
    namespace Tooltips {
        constexpr float TooltipWidth = 150.0f;

        inline void DrawUnloadedResourceTooltip(const fs::path& path, const char* resourceName) {
            if (!ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                return;

            ImGui::BeginTooltip();
            ImUtil::TextShifted(ImUtil::TruncateText(
                path.filename().generic_string().c_str(),
                TooltipWidth, false
            ).c_str(), 0.5f);
            ImGui::TextUnformatted(fmt::format("Unloaded {}", resourceName).c_str());
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
            ImGui::TextUnformatted(fmt::format("Shader resource #{}", shader.id()).c_str());
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
            ImGui::TextUnformatted(fmt::format("Texture resource #{}", texture.id()).c_str());
            ImGui::TextUnformatted(fmt::format(
                "{}x{} pixels",
                texture->dimensions().width,
                texture->dimensions().height
            ).c_str());
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
            ImGui::TextUnformatted(fmt::format("Mesh resource #{}", mesh.id()).c_str());
            ImGui::TextUnformatted(fmt::format(
                "{} triangle{}, {} vertices{}",
                mesh->trianglesCount(), mesh->trianglesCount() == 1 ? "" : "s",
                mesh->verticesCount(), mesh->verticesCount() == 1 ? "" : "s"
            ).c_str());
            ImGui::EndTooltip();
        }
    }
}


} // namespace rf
