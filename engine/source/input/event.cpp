#include "rf/input/event.hpp"

#include <GLFW/glfw3.h>

namespace rf {

const char* Input::KeyEventName(KeyEvent event) {
    switch (event) {
        case KeyEvent::Press:   return "Press";
        case KeyEvent::Release: return "Release";
        case KeyEvent::Repeat:  return "Repeat";
        default:                return "[Unknown Key Event]";
    }
}

Input::KeyEvent Input::GlfwMacroToKeyEvent(int event) {
    switch (event) {
        case GLFW_PRESS:    return KeyEvent::Press;
        case GLFW_RELEASE:  return KeyEvent::Release;
        case GLFW_REPEAT:   return KeyEvent::Repeat;
        default:            return KeyEvent::None;
    }
}

int Input::KeyEventToGlfwMacro(KeyEvent event) {
    switch (event) {
        case KeyEvent::Press:   return GLFW_PRESS;
        case KeyEvent::Release: return GLFW_RELEASE;
        case KeyEvent::Repeat:  return GLFW_REPEAT;
        default:                return -1;
    }
}

} // namespace rf
