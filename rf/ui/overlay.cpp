#include "overlay.hpp"

#include <utility>

#include <rf/ui/fonts/b612_regular_ttf.hpp>
#include <rf/ui/windows/bindings.hpp>
#include <rf/ui/windows/perfomance.hpp>
#include <rf/ui/font.hpp>

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

    ImFontConfig config = ImFontConfig();
    config.FontDataOwnedByAtlas = false;

    const auto& font = Fonts::B612_Regular_Ttf();
    io.Fonts->AddFontFromMemoryTTF(
        const_cast<void*>(reinterpret_cast<const void*>(font.data())),
        static_cast<int>(font.size()),
        FontSize(Font::Normal),
        &config
    );
    io.Fonts->AddFontFromMemoryTTF(
        const_cast<void*>(reinterpret_cast<const void*>(font.data())),
        static_cast<int>(font.size()),
        FontSize(Font::Huge),
        &config
    );
    
    m_windows.push_back(std::make_unique<Windows::Bindings>());
    m_windows.push_back(std::make_unique<Windows::Perfomance>());
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
        if (window->size().x * window->size().y)
            ImGui::SetNextWindowSize(window->size());
        if (ImGui::Begin(window->name(), nullptr, window->flags()))
            window->update();
        ImGui::End();
    }

    static bool s_firstRender = true;
    if (s_firstRender) {
        ImGui::SetWindowFocus(nullptr);
        s_firstRender = false;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace rf
