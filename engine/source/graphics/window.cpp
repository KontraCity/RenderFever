#include "rf/graphics/window.hpp"

#include "rf/core/engine.hpp"
#include "rf/core/error.hpp"

namespace rf {

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input::Map& map = Engine::InputMap();
    map.broadcastKeyEvent(key, action);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Input::Map& map = Engine::InputMap();
    map.broadcastKeyEvent(button, action);
}

static void CursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition) {
    Input::Map& map = Engine::InputMap();
    map.broadcastCursorMoveEvent(xPosition, yPosition);
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Input::Map& map = Engine::InputMap();
    map.broadcastScrollEvent(xOffset, yOffset);
}

void Graphics::Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* root = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    root->m_dimensions.width = static_cast<GLsizei>(width);
    root->m_dimensions.height = static_cast<GLsizei>(height);
    glViewport(0, 0, root->m_dimensions.width, root->m_dimensions.height);
}

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
    glfwSetKeyCallback(m_window, &KeyCallback);
    glfwSetMouseButtonCallback(m_window, &MouseButtonCallback);
    glfwSetCursorPosCallback(m_window, &CursorMoveCallback);
    glfwSetScrollCallback(m_window, &ScrollCallback);
    glfwSetFramebufferSizeCallback(m_window, &Window::FrameBufferSizeCallback);

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

} // namespace rf
