#include "imutil.hpp"

#include <rf/auxiliary/imgui.hpp>

namespace rf {

void Ui::ImUtil::TextUnformattedShiftedRight(const char* text, float ratio) {
    ImVec2 regionSize = ImGui::GetContentRegionAvail();
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (regionSize.x - textSize.x) * ratio);
    ImGui::TextUnformatted(text);
}

} // namespace rf
