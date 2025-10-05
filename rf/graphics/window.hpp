#pragma once

#include <string>

#include <rf/auxiliary/gl.hpp>

namespace rf {

namespace Graphics {
    struct Dimensions {
        GLsizei width = 0;
        GLsizei height = 0;
    };

    enum class CursorMode {
        Normal,     // Cursor hovers over the window, can escape borders
        Hidden,     // The same as Normal, but invisible while in borders
        Captured,   // The same as Normal, but can't escape borders
        Disabled,   // There is no cursor and all mouse movements are captured
    };

    class Window {
    public:
        struct Config {
            std::string title     = "RenderFever Engine";
            Dimensions dimensions = { 1280, 720 };
            CursorMode cursorMode = CursorMode::Disabled;
            bool vSync            = true;
        };

    private:
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_handle = nullptr;
        std::string m_title;
        Dimensions m_dimensions = {};
        CursorMode m_cursorMode = CursorMode::Normal;
        bool m_vSync = true;

    public:
        Window(const Config& config);

        Window(const Window& other) = delete;

        Window(Window&& other) noexcept;

        ~Window();

    public:
        Window& operator=(const Window& other) = delete;

        Window& operator=(Window&& other) noexcept;

    public:
        void swapBuffers() {
            glfwSwapBuffers(m_handle);
        }

    public:
        // TODO: The only user of this is the Engine. Use friends?
        GLFWwindow* handle() {
            return m_handle;
        }

        bool isMinimized() const {
            return glfwGetWindowAttrib(m_handle, GLFW_ICONIFIED);
        }

        const std::string& getTitle() const {
            return m_title;
        }

        void setTitle(const std::string& title) {
            m_title = title;
            glfwSetWindowTitle(m_handle, title.c_str());
        }

        Dimensions getDimensions() const {
            return m_dimensions;
        }

        void setDimensions(const Dimensions& dimensions) {
            m_dimensions = dimensions;
            glfwSetWindowSize(m_handle, dimensions.width, dimensions.height);
        }

        CursorMode getCursorMode() const {
            return m_cursorMode;
        }

        void setCursorMode(CursorMode cursorMode, bool resetLastCursorPosition = true);

        bool getVSync() const {
            return m_vSync;
        }

        void setVSync(bool vSync) {
            m_vSync = vSync;
            glfwSwapInterval(vSync ? 1 : 0);
        }

        bool getShouldClose() const {
            return glfwWindowShouldClose(m_handle);
        }

        void setShouldClose(bool shouldClose) {
            glfwSetWindowShouldClose(m_handle, shouldClose);
        }
    };
}

} // namespace rf
