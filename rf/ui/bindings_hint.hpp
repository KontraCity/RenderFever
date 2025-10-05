#pragma once

#include <rf/auxiliary/imgui.hpp>
#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    class BindingsHint : public Window {
    public:
        virtual const char* name() const {
            return "Bindings Hint";
        }

        virtual ImGuiWindowFlags flags() const {
            return ImGuiWindowFlags_NoResize;
        }

        virtual void draw() const;
    };
}

} // namespace rf
