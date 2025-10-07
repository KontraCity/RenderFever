#pragma once

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    namespace ImUtil {
        // Align: left = 0.0f, center = 0.5f, right = 1.0f, etc.
        inline void TextShiftedRight(const char* text, float ratio) {
            ImVec2 regionSize = ImGui::GetContentRegionAvail();
            ImVec2 textSize = ImGui::CalcTextSize(text);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (regionSize.x - textSize.x) * ratio);
            ImGui::TextUnformatted(text);
        }
    }
}

} // namespace rf
