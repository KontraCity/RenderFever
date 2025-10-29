#pragma once

#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    namespace Windows {
        class BindingsHint : public Window {
        public:
            virtual void update();

        public:
            virtual const char* name() const {
                return "Bindings Hint";
            }

            virtual ImGuiWindowFlags flags() const {
                return ImGuiWindowFlags_AlwaysAutoResize;
            }
        };
    }
}

} // namespace rf
