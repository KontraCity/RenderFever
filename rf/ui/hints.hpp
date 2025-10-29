#pragma once

#include <fmt/format.h>

#include <rf/auxiliary/imgui.hpp>

#include <rf/core/engine.hpp>
#include <rf/resources/library.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

namespace Ui {
    namespace Hints {
        constexpr float HintWidth = 75.0f;

        inline void DrawShaderHint(const Resources::Shader& shader, bool raw = false) {
            const Graphics::Texture& shaderPreview = Engine::Overlay().previewMap().getShaderPreview(shader);
            ImGui::Image(shaderPreview.handle(), ImVec2(HintWidth, HintWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                shader.path().filename().generic_string().c_str(),
                HintWidth, true
            ).c_str(), 0.5f);
        }

        inline void DrawTextureHint(const Resources::Texture& texture) {
            ImGui::Image(texture->handle(), ImVec2(HintWidth, HintWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                texture.path().filename().generic_string().c_str(),
                HintWidth, true
            ).c_str(), 0.5f);
        }
    
        inline void DrawMeshHint(const Resources::Mesh& mesh) {
            const Graphics::Texture& meshPreview = Engine::Overlay().previewMap().getMeshPreview(mesh);
            ImGui::Image(meshPreview.handle(), ImVec2(HintWidth, HintWidth));
            ImUtil::TextShifted(ImUtil::TruncateText(
                mesh.path().filename().generic_string().c_str(),
                HintWidth, true
            ).c_str(), 0.5f);
        }
    }
}

} // namespace rf
