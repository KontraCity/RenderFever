#pragma once

#include <vector>

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    namespace Styles {
        struct ColorStyle {
            ImGuiCol type;
            ImVec4 color;
        };
        using ColorStyleList = std::vector<ColorStyle>;

        const ColorStyleList None = {};

        const ColorStyleList InvisibleButton = {
            { ImGuiCol_Button,          ImVec4(0.0f, 0.0f, 0.0f, 0.0f) },
            { ImGuiCol_ButtonHovered,   ImVec4(1.0f, 1.0f, 1.0f, 0.1f) },
            { ImGuiCol_ButtonActive,    ImVec4(1.0f, 1.0f, 1.0f, 0.2f) },
        };

        const ColorStyleList ErrorText = {
            { ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f) },
        };

        inline void WithColorStyle(const ColorStyleList& styles, const std::function<void()>& drawContent) {
            for (const ColorStyle& style : styles)
                ImGui::PushStyleColor(style.type, style.color);
            drawContent();
            if (!styles.empty())
                ImGui::PopStyleColor(styles.size());
        }
    }
}

} // namespace rf
