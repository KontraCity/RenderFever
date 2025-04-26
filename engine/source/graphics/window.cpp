#include "rf/graphics/window.hpp"

#include "rf/core/error.hpp"

namespace rf {

Graphics::Window::Window(const std::string& title, const Dimensions& dimensions)
    : m_title(title)
    , m_dimensions(dimensions) {
    if (glfwInit() != GLFW_TRUE)
        throw RF_LOCATED_ERROR("Couldn't initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_dimensions.width, m_dimensions.height, title.c_str(), NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    GLenum result = glewInit();
    if (result != GLEW_OK) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't initialize GLEW");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, m_dimensions.width, m_dimensions.height);

    
}

Graphics::Window::~Window() {
    glfwTerminate();
}

void Graphics::Window::update() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Graphics::Window::rename(const std::string& title) {
    glfwSetWindowTitle(m_window, title.c_str());
}

void Graphics::Window::resize(const Dimensions& dimensions) {
    glfwSetWindowSize(m_window, dimensions.width, dimensions.height);
}

} // namespace rf
