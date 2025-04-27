#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace rf {

namespace Graphics {
    class Window {
    public:
        struct Dimensions {
            GLsizei width = 0;
            GLsizei height = 0;

            inline bool operator==(const Dimensions& other) {
                return width == other.width && height == other.height;
            }
        };

    private:
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_window = nullptr;
        std::string m_title;
        Dimensions m_dimensions;

    public:
        Window(const std::string& title, const Dimensions& dimensions);

        Window(const Window& other) = delete;

        Window(Window&& other) = delete;

        ~Window();

    public:
        inline void update() {
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        inline void rename(const std::string& title) {
            if (title == m_title)
                return;
            m_title = title;
            glfwSetWindowTitle(m_window, title.c_str());
        }

        inline void resize(const Dimensions& dimensions) {
            if (dimensions == m_dimensions)
                return;
            m_dimensions = dimensions;
            glfwSetWindowSize(m_window, dimensions.width, dimensions.height);
        }

        inline void setShouldClose() {
            glfwSetWindowShouldClose(m_window, true);
        }

    public:
        inline const std::string& title() const {
            return m_title;
        }
        
        inline const Dimensions& dimensions() const {
            return m_dimensions;
        }

        inline bool shouldClose() const {
            return glfwWindowShouldClose(m_window);
        }
    };
}

} // namespace rf
