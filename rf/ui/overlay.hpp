#pragma once

#include <vector>

#include <rf/auxiliary/gl.hpp>
#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/icon_map.hpp>
#include <rf/ui/preview_map.hpp>
#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    class Overlay {
    private:
        GLFWwindow* m_handle = nullptr;
        std::vector<Window::Instance> m_windows;
        IconMap m_iconMap;
        PreviewMap m_previewMap;

    public:
        Overlay(GLFWwindow* handle);

        Overlay(const Overlay& other) = delete;

        Overlay(Overlay&& other) noexcept;

        ~Overlay();

    public:
        Overlay& operator=(const Overlay& other) = delete;

        Overlay& operator=(Overlay&& other) noexcept;

    public:
        void render() const;

    public:
        const IconMap& iconMap() const {
            return m_iconMap;
        }

        PreviewMap& previewMap() {
            return m_previewMap;
        }

        bool isMouseWanted() const {
            return ImGui::GetIO().WantCaptureMouse;
        }

        void setMouseDisabled(bool mouseDisabled) {
            if (mouseDisabled)
                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
            else
                ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
    };
}

} // namespace rf
