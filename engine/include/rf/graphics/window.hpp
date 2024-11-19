#pragma once

// Graphics libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Custom engine modules
#include "rf/engine/input.hpp"
#include "rf/engine/stopwatch.hpp"
#include "rf/engine/utility.hpp"

// Custom graphics modules
#include "rf/graphics/camera.hpp"
#include "rf/graphics/cube.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics
{
    class Window
    {
    private:
        /// @brief GLFW framebuffer resize event callback
        /// @param window The window that received the event
        /// @param width New width
        /// @param height New height
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        /* Window specific */
        spdlog::logger m_logger;
        GLFWwindow* m_window = nullptr;
        int m_width = 0;
        int m_height = 0;
        Engine::Stopwatch m_stopwatch;

        /* Resources */
        Camera m_camera;
        Shader m_shader;
        Texture::Pointer m_texture;

        /* Variables */
        float m_currentFrameTime = 0.0;
        float m_deltaTime = 0.0;
        float m_lastFrameTime = 0.0;

    public:
        /// @brief Create window and prepare for run loop
        /// @param width Window width
        /// @param height Window height
        /// @param resourcesPath Path to resources directory
        /// @throw std::runtime_error if internal error occurs
        Window(unsigned int width, unsigned int height, const std::string& resourcesPath);

        Window(const Window& other) = delete;

        Window(Window&& other) = delete;

        ~Window();

    private:
        /// @brief Process single key event
        /// @param key The key event is associated with
        /// @param action Key action (press/repeat/release)
        /// @param mods Bit field describing which modifier keys were held down
        void onKey(int key, int action, int mods);

        /// @brief Toggle wireframe rendering mode
        void toggleWireframe();

        /// @brief Toggle VSync frame limiter
        void toggleVSync();

        /// @brief Show FPS to console
        void showFps() const;

    public:
        /// @brief Start run loop
        void run();
    };
}

} // namespace rf
