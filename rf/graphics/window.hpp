#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace rf {

class Window {
public:
    class Dimensions {
    private:
        GLsizei m_width = 0;
        GLsizei m_height = 0;

    public:
        Dimensions() = default;

        Dimensions(GLsizei width, GLsizei height)
            : m_width(width)
            , m_height(height)
        {}

    public:
        bool operator==(const Dimensions& other) const {
            return m_width == other.m_width && m_height == other.m_height;
        }

    public:
        float evaluateRatio() const {
            return static_cast<float>(m_width) / m_height;
        }

    public:
        GLsizei width() const {
            return m_width;
        }

        GLsizei& width() {
            return m_width;
        }

        GLsizei height() const {
            return m_height;
        }

        GLsizei& height() {
            return m_height;
        }
    };

    enum class CursorMode {
        Normal,     // Cursor hovers over the window, can escape borders
        Hidden,     // The same as Normal, but invisible while in borders
        Captured,   // The same as Normal, but can't escape borders
        Disabled,   // There is no cursor and all mouse movements are captured
    };

private:
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    static int CursorModeToGlfwMacro(CursorMode cursorMode);

private:
    GLFWwindow* m_handle = nullptr;
    bool m_vSync = true;
    std::string m_title;
    Dimensions m_dimensions;
    CursorMode m_cursorMode = CursorMode::Normal;

public:
    Window(const std::string& title, const Dimensions& dimensions);

    Window(const Window& other) = delete;

    Window(Window&& other) = delete;

    ~Window();

public:
    void swapBuffers();

public:
    void setTitle(const std::string& title);

    void setDimensions(const Dimensions& dimensions);

    void setVSync(bool vSync);

    void setShouldClose(bool shouldClose);

    void setCursorMode(CursorMode cursorMode);

public:
    const std::string& getTitle() const {
        return m_title;
    }
        
    const Dimensions& getDimensions() const {
        return m_dimensions;
    }

    bool getVSync() const {
        return m_vSync;
    }

    bool getShouldClose() const {
        return glfwWindowShouldClose(m_handle);
    }

    CursorMode getCursorMode() const {
        return m_cursorMode;
    }
};

} // namespace rf
