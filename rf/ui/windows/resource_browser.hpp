#pragma once

#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    namespace Windows {
        class ResourceBrowser : public Window {
        public:
            virtual void update();

        public:
            virtual const char* name() const {
                return "Resource Browser";
            }

            virtual ImVec2 size() const {
                return ImVec2(800, 440);
            }
        };
    }
}

} // namespace rf
