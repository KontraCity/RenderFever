#include "rf/graphics/window.hpp"

#include "rf/core/engine.hpp"
#include "rf/core/error.hpp"

namespace rf {

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Map& map = Engine::InputMap();
    map.broadcastKeyEvent(key, action);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Map& map = Engine::InputMap();
    map.broadcastKeyEvent(button, action);
}

static void CursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition) {
    Map& map = Engine::InputMap();
    map.broadcastCursorMoveEvent(xPosition, yPosition);
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Map& map = Engine::InputMap();
    map.broadcastScrollEvent(xOffset, yOffset);
}

void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* root = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    root->m_dimensions.width() = static_cast<GLsizei>(width);
    root->m_dimensions.height() = static_cast<GLsizei>(height);
    glViewport(0, 0, root->m_dimensions.width(), root->m_dimensions.height());
}

int Window::CursorModeToGlfwMacro(CursorMode cursorMode) {
    switch (cursorMode) {
        case CursorMode::Normal:    return GLFW_CURSOR_NORMAL;
        case CursorMode::Hidden:    return GLFW_CURSOR_HIDDEN;
        case CursorMode::Captured:  return GLFW_CURSOR_CAPTURED;
        case CursorMode::Disabled:  return GLFW_CURSOR_DISABLED;
        default:                    return -1;
    }
}

Window::Window(const std::string& title, const Dimensions& dimensions)
    : m_title(title)
    , m_dimensions(dimensions) {
    if (glfwInit() != GLFW_TRUE)
        throw RF_LOCATED_ERROR("Couldn't initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_handle = glfwCreateWindow(m_dimensions.width(), m_dimensions.height(), title.c_str(), NULL, NULL);
    if (!m_handle) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't create GLFW window");
    }
    glfwMakeContextCurrent(m_handle);
    glfwSetWindowUserPointer(m_handle, this);
    glfwSetKeyCallback(m_handle, &KeyCallback);
    glfwSetMouseButtonCallback(m_handle, &MouseButtonCallback);
    glfwSetCursorPosCallback(m_handle, &CursorMoveCallback);
    glfwSetScrollCallback(m_handle, &ScrollCallback);
    glfwSetFramebufferSizeCallback(m_handle, &Window::FrameBufferSizeCallback);

    GLenum result = glewInit();
    if (result != GLEW_OK) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't initialize GLEW");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, m_dimensions.width(), m_dimensions.height());
}

Window::~Window() {
    glfwTerminate();
}

void Window::update() {
    glfwSwapBuffers(m_handle);
    glfwPollEvents();
}

void Window::rename(const std::string& title) {
    if (title == m_title)
        return;
    m_title = title;
    glfwSetWindowTitle(m_handle, title.c_str());
}

void Window::resize(const Dimensions& dimensions) {
    if (dimensions == m_dimensions)
        return;
    m_dimensions = dimensions;
    glfwSetWindowSize(m_handle, dimensions.width(), dimensions.height());
}

void Window::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(m_handle, shouldClose);
}

void Window::setCursorMode(CursorMode cursorMode) {
    if (m_cursorMode == cursorMode)
        return;
    m_cursorMode = cursorMode;
    glfwSetInputMode(m_handle, GLFW_CURSOR, CursorModeToGlfwMacro(cursorMode));
    Engine::InputMap().resetLastCursorPosition();
}

} // namespace rf
