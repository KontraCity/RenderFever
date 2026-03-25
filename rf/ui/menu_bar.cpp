#include "menu_bar.hpp"

#include <string>

#include <rf/core/engine.hpp>

#include <rf/auxiliary/imgui.hpp>

namespace rf {

Ui::MenuBar::MenuBar(const Window::SharedList& windows)
    : m_windows(windows)
{}

void Ui::MenuBar::updateShortcuts() {
    ImGuiIO io = ImGui::GetIO();
    if (io.KeyAlt) {
        if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false))
            m_allWindowsHidden = !m_allWindowsHidden;

        for (size_t index = 0, size = m_windows->size(); index < size; ++index) {
            Window::Instance& window = m_windows->at(index);
            if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(ImGuiKey_1 + index), false))
                window->setIsOpen(!window->isOpen());
        }
    } 
}

void Ui::MenuBar::updateWindow() {
    if (!ImGui::BeginMainMenuBar())
        return;
    
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit", "Alt+F4"))
            Engine::Exit();
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window")) {
        if (ImGui::MenuItem("Hide all", "Alt+`", m_allWindowsHidden))
            m_allWindowsHidden = !m_allWindowsHidden;

        if (ImGui::BeginMenu("Open windows")) {
            for (size_t index = 0, size = m_windows->size(); index < size; ++index) {
                Window::Instance& window = m_windows->at(index);
                std::string shortcut = "Alt+" + std::to_string(index + 1);
                if (ImGui::MenuItem(window->name(), shortcut.c_str(), window->isOpen()))
                    window->setIsOpen(!window->isOpen());
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void Ui::MenuBar::update() {
    updateShortcuts();
    updateWindow();
}

} // namespace rf
