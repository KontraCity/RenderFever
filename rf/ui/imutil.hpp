#pragma once

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    namespace ImUtil {
        // X shift: left = 0.0f, center = 0.5f, right = 1.0f.
        // Y shift: up = 0.0f, center = 0.5f, bottom = 1.0f.
        inline void TextShifted(const char* text, float xRatio, float yRatio = 0.0f) {
            ImVec2 regionSize = ImGui::GetContentRegionAvail();
            ImVec2 textSize = ImGui::CalcTextSize(text);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (regionSize.x - textSize.x) * xRatio);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (regionSize.y - textSize.y) * yRatio);
            ImGui::TextUnformatted(text);
        }
    }
}

} // namespace rf
