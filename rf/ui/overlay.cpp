#include "overlay.hpp"

#include <utility>

#include <rf/ui/bindings_hint.hpp>

namespace rf {

Ui::Overlay::Overlay(GLFWwindow* handle) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(handle, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    m_windows.push_back(std::make_unique<BindingsHint>());
}

Ui::Overlay::Overlay(Overlay&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr))
    , m_windows(std::move(other.m_windows))
{}

Ui::Overlay::~Overlay() {
    if (!m_handle)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Ui::Overlay& Ui::Overlay::operator=(Overlay&& other) noexcept {
    if (this != &other) {
        m_handle = std::exchange(other.m_handle, nullptr);
        m_windows = std::move(other.m_windows);
    }
    return *this;
}

void Ui::Overlay::render() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (const Window::Instance& window : m_windows) {
        if (ImGui::Begin(window->name(), nullptr, window->flags()))
            window->draw();
        ImGui::End();
    }

    static bool firstRender = true;
    if (firstRender) {
        ImGui::SetWindowFocus(nullptr);
        firstRender = false;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace rf
