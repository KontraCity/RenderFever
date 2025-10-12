#pragma once

#include <rf/auxiliary/imgui.hpp>

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    namespace Windows {
        class Scene : public Window {
        public:
            virtual void update();

        public:
            virtual const char* name() const {
                return "Scene";
            }

            virtual ImGuiWindowFlags flags() const {
                return 0;
            }

            virtual ImVec2 size() const {
                return ImVec2(500, 440);
            }
        };
    }
}

} // namespace rf
