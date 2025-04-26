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
        void update();

        void rename(const std::string& title);

        void resize(const Dimensions& dimensions);

    public:
        inline const std::string& title() {
            return m_title;
        }
        
        inline const Dimensions& dimensions() {
            return m_dimensions;
        }
    };
}

} // namespace rf
