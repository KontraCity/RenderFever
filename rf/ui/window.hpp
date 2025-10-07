#pragma once

#include <memory>

#include <rf/auxiliary/imgui.hpp>

namespace rf {

namespace Ui {
    class Window {
    public:
        using Instance = std::unique_ptr<Window>;

    public:
        virtual ~Window() = default;

    public:
        virtual void update() = 0;

    public:
        virtual const char* name() const = 0;

        virtual ImGuiWindowFlags flags() const {
            return 0;
        }

        virtual ImVec2 size() const {
            return ImVec2(0, 0);
        }
    };
}

} // namespace rf
