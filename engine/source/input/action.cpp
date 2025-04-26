#include "rf/input/action.hpp"

#include <GLFW/glfw3.h>

namespace rf {

const char* Input::ActionName(Action action) {
    switch (action) {
        case Action::Press:
            return "Press";
        case Action::Release:
            return "Release";
        case Action::Repeat:
            return "Repeat";
        default:
            return "[Unknown Action]";
    }
}

Input::Action Input::GlfwMacroToAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return Action::Press;
        case GLFW_RELEASE:
            return Action::Release;
        case GLFW_REPEAT:
            return Action::Repeat;
        default:
            return Action::None;
    }
}

int Input::ActionToGlfwMacro(Action action) {
    switch (action) {
        case Action::Press:
            return GLFW_PRESS;
        case Action::Release:
            return GLFW_RELEASE;
        case Action::Repeat:
            return GLFW_REPEAT;
        default:
            return -1;
    }
}

} // namespace rf
