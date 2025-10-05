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
        virtual const char* name() const = 0;

        virtual ImGuiWindowFlags flags() const {
            return 0;
        }

        virtual void draw() const = 0;
    };
}

} // namespace rf
