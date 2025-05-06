#include "rf/inputs/event.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rf/core/engine.hpp"

namespace rf {

const char* Inputs::KeyEventName(KeyEvent event) {
    switch (event) {
        case KeyEvent::Press:   return "Press";
        case KeyEvent::Release: return "Release";
        case KeyEvent::Repeat:  return "Repeat";
        default:                return "[Unknown Key Event]";
    }
}

Inputs::KeyEvent Inputs::GlfwMacroToKeyEvent(int event) {
    switch (event) {
        case GLFW_PRESS:    return KeyEvent::Press;
        case GLFW_RELEASE:  return KeyEvent::Release;
        case GLFW_REPEAT:   return KeyEvent::Repeat;
        default:            return KeyEvent::None;
    }
}

int Inputs::KeyEventToGlfwMacro(KeyEvent event) {
    switch (event) {
        case KeyEvent::Press:   return GLFW_PRESS;
        case KeyEvent::Release: return GLFW_RELEASE;
        case KeyEvent::Repeat:  return GLFW_REPEAT;
        default:                return -1;
    }
}

bool Inputs::KeyPressed(Input input) {
    if (input == Input::None || IsSpecialInput(input)) {
        // Unknown or special inputs can't be pressed
        return false;
    }

    int status = glfwGetKey(Engine::Window().handle(), InputToGlfwMacro(input));
    return GlfwMacroToKeyEvent(status) == KeyEvent::Press;
}

} // namespace rf
