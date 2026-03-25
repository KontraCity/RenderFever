#pragma once

#include <memory>
#include <vector>

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    class Window {
    public:
        using Instance = std::unique_ptr<Window>;
        using List = std::vector<Instance>;
        using SharedList = std::shared_ptr<List>;

    private:
        bool m_isOpen = true;

    public:
        virtual ~Window() = default;

    private:
        virtual void updateWindow() = 0;

    public:
        virtual void update() {
            if (!m_isOpen)
                return;

            ImVec2 windowSize = size();
            if (windowSize.x * windowSize.y)
                ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

            if (ImGui::Begin(name(), nullptr, flags()))
                updateWindow();
            ImGui::End();
        }

    public:
        virtual const char* name() const = 0;

        virtual ImGuiWindowFlags flags() const {
            return 0;
        }

        virtual ImVec2 size() const {
            return ImVec2(0, 0);
        }

        virtual bool isOpen() const {
            return m_isOpen;
        }

        virtual void setIsOpen(bool isOpen) {
            m_isOpen = isOpen;
        }
    };
}

} // namespace rf
