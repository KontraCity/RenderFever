#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include "rf/core/stopwatch.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/cubemap.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    class Window {
    private:
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        spdlog::logger m_logger;
        GLFWwindow* m_window = nullptr;
        int m_width = 0;
        int m_height = 0;
        Core::Stopwatch m_stopwatch;

        Camera m_camera;
        Shader m_shader;
        Shader m_lightingShader;
        Shader m_skyboxShader;
        Shader m_normalShader;
        Material m_containerMaterial;
        Cubemap::Pointer m_skyboxCubemap;

        float m_currentFrameTime = 0.0;
        float m_deltaTime = 0.0;
        float m_lastFrameTime = 0.0;
        bool m_vsync = true;
        bool m_showNormals = false;
        bool m_flashlight = false;

    public:
        Window(unsigned int width, unsigned int height);

        Window(const Window& other) = delete;

        Window(Window&& other) noexcept = delete;

        ~Window();

    private:
        void onKey(int key, int action, int mods);

        void buildShaders();

        void toggleWireframe();

        void toggleVSync();

        void toggleAntiAliasing();

    public:
        void run();
    };
}

} // namespace rf
