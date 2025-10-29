#pragma once

#include <cstring>

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

        inline std::string TruncateText(const char* text, float maxWidth, bool truncateAtEnd = true) {
            ImVec2 textSize = ImGui::CalcTextSize(text);
            if (textSize.x < maxWidth)
                return text;

            ImVec2 dotsSize = ImGui::CalcTextSize("...");
            if (maxWidth <= dotsSize.x)
                return "...";

            int length = static_cast<int>(std::strlen(text)), left = 0, right = length;
            while (left < right) {
                int middle = (left + right + 1) / 2;
                if (!truncateAtEnd)
                    textSize = ImGui::CalcTextSize(text + (length - middle));
                else
                    textSize = ImGui::CalcTextSize(text, text + middle);

                if (textSize.x + dotsSize.x <= maxWidth)
                    left = middle;
                else
                    right = middle - 1;
            }

            if (!truncateAtEnd)
                return "..." + std::string(text + (length - left));
            else
                return std::string(text, text + left) + "...";
        }
    
        template <typename Target>
        inline bool SendDragDropPayload(const char* type, const Target& target, const std::function<void()>& drawDragDropHint) {
            bool hintDrawn = ImGui::BeginDragDropSource();
            if (hintDrawn) {
                ImGui::SetDragDropPayload(type, &target, sizeof(Target));
                drawDragDropHint();
                ImGui::EndDragDropSource();
            }
            return hintDrawn;
        }

        template <typename Target>
        inline void AcceptDragDropPayload(const char* type, Target& target) {
            if (ImGui::BeginDragDropTarget()) {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
                if (payload && payload->DataSize == sizeof(Target))
                    target = *reinterpret_cast<Target*>(payload->Data);
                ImGui::EndDragDropTarget();
            }
        }
    }
}

} // namespace rf
