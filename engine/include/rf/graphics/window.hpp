#pragma once

// Graphics libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Library spdlog
#include <spdlog/spdlog.h>

// Custom engine modules
#include "rf/engine/stopwatch.hpp"

// Custom graphics modules
#include "rf/graphics/camera.hpp"
#include "rf/graphics/cubemap.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/model.hpp"
#include "rf/graphics/shader.hpp"

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
        Shader m_lightingShader;
        Shader m_skyboxShader;
        Shader m_normalShader;
        Material m_containerMaterial;
        Model m_backpack;
        Cubemap::Pointer m_skyboxCubemap;

        /* Variables */
        float m_currentFrameTime = 0.0;
        float m_deltaTime = 0.0;
        float m_lastFrameTime = 0.0;
        bool m_vsync = true;
        bool m_showNormals = false;
        bool m_flashlight = false;

    public:
        /// @brief Create window and prepare for run loop
        /// @param width Window width
        /// @param height Window height
        /// @throw std::runtime_error if internal error occurs
        Window(unsigned int width, unsigned int height);

        Window(const Window& other) = delete;

        Window(Window&& other) noexcept = delete;

        ~Window();

    private:
        /// @brief Process single key event
        /// @param key The key event is associated with
        /// @param action Key action (press/repeat/release)
        /// @param mods Bit field describing which modifier keys were held down
        void onKey(int key, int action, int mods);

        /// @brief Build/rebuild shaders
        void buildShaders();

        /// @brief Toggle wireframe rendering mode
        void toggleWireframe();

        /// @brief Toggle VSync frame limiter
        void toggleVSync();

        /// @brief Toggle anti aliasing
        void toggleAntiAliasing();

    public:
        /// @brief Start run loop
        void run();
    };
}

} // namespace rf
