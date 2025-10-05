#pragma once

#include <vector>

#include <rf/auxiliary/gl.hpp>
#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    class Overlay {
    private:
        GLFWwindow* m_handle = nullptr;
        std::vector<Window::Instance> m_windows;

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
