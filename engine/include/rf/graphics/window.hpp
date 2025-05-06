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

            float ratio() const {
                return static_cast<float>(width) / height;
            }

            bool operator==(const Dimensions& other) const {
                return width == other.width && height == other.height;
            }
        };

    private:
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* m_handle = nullptr;
        std::string m_title;
        Dimensions m_dimensions;

    public:
        Window(const std::string& title, const Dimensions& dimensions);

        Window(const Window& other) = delete;

        Window(Window&& other) = delete;

        ~Window();

    public:
        void update() {
            glfwSwapBuffers(m_handle);
            glfwPollEvents();
        }

        void rename(const std::string& title) {
            if (title == m_title)
                return;
            m_title = title;
            glfwSetWindowTitle(m_handle, title.c_str());
        }

        void resize(const Dimensions& dimensions) {
            if (dimensions == m_dimensions)
                return;
            m_dimensions = dimensions;
            glfwSetWindowSize(m_handle, dimensions.width, dimensions.height);
        }

        void setShouldClose() {
            glfwSetWindowShouldClose(m_handle, true);
        }

    public:
        GLFWwindow* handle() {
            return m_handle;
        }

        const std::string& title() const {
            return m_title;
        }
        
        const Dimensions& dimensions() const {
            return m_dimensions;
        }

        bool shouldClose() const {
            return glfwWindowShouldClose(m_handle);
        }
    };
}

} // namespace rf
