#pragma once

// Graphics libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Custom modules
#include "rf/engine/dispatcher.hpp"

namespace rf {

namespace Engine
{
    namespace Input
    {
        /// @brief Single key event dispatcher
        /// @param (int) key The key event is associated with
        /// @param (int) action Key action (press/repeat/release)
        /// @param (int) mods Bit field describing which modifier keys were held down
        inline Dispatcher<int, int, int> Key;

        /// @brief Continuous key event dispatcher
        /// @param (GLFWwindow*) window The window that received the event
        /// @param (float) deltaTime Time passed from last frame
        inline Dispatcher<GLFWwindow*, float> Input;

        /// @brief Mouse cursor position event dispatcher
        /// @param (double) x New cursor X coordinate
        /// @param (double) y New cursor Y coordinate
        inline Dispatcher<double, double> CursorPos;

        /// @brief Mouse scroll event dispatcher
        /// @param (double) xOffset X coordinate scroll offset
        /// @param (double) yOffset y coordinate scroll offset
        inline Dispatcher<double, double> Scroll;
    }
}

} // namespace rf
