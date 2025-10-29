#pragma once

#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    namespace Windows {
        class PerfomanceMonitor : public Window {
        public:
            virtual void update();

        public:
            virtual const char* name() const {
                return "Perfomance Monitor";
            }

            virtual ImGuiWindowFlags flags() const {
                return ImGuiWindowFlags_NoResize;
            }

            virtual ImVec2 size() const {
                return ImVec2(365, 174);
            }
        };
    }
}

} // namespace rf
