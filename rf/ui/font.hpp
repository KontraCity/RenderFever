#pragma once

#include <functional>
#include <utility>

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    enum class Font : int {
        Normal = 0,
        Huge   = 1,
    };

    constexpr float FontSize(Font font) {
        constexpr float BaseSize = 14.0f;
        switch (font) {
            case Font::Normal:  return BaseSize * 1;
            case Font::Huge:    return BaseSize * 3;
            default:            return 0.0f;
        }
    }

    inline void WithFont(Font font, const std::function<void()>& function) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[std::to_underlying(font)]);
        function();
        ImGui::PopFont();
    }
}

} // namespace rf
