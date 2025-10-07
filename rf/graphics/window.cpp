#include "window.hpp"

#include <utility>

#include <rf/core/engine.hpp>
#include <rf/core/error.hpp>

namespace rf {

constexpr int CursorModeToGlfwMacro(Graphics::CursorMode cursorMode) {
    switch (cursorMode) {
        case Graphics::CursorMode::Normal:      return GLFW_CURSOR_NORMAL;
        case Graphics::CursorMode::Hidden:      return GLFW_CURSOR_HIDDEN;
        case Graphics::CursorMode::Captured:    return GLFW_CURSOR_CAPTURED;
        case Graphics::CursorMode::Disabled:    return GLFW_CURSOR_DISABLED;
        default:                                return -1;
    }
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input::InputMap& inputMap = Engine::InputMap();
    inputMap.broadcastKeyEvent(key, action);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Input::InputMap& inputMap = Engine::InputMap();
    inputMap.broadcastKeyEvent(button, action);
}

static void CursorMoveCallback(GLFWwindow* window, double xPosition, double yPosition) {
    Input::InputMap& inputMap = Engine::InputMap();
    inputMap.broadcastCursorMoveEvent(xPosition, yPosition);
}

static void CursorScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    Input::InputMap& inputMap = Engine::InputMap();
    inputMap.broadcastCursorScrollEvent(xOffset, yOffset);
}

void Graphics::Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* root = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    root->m_dimensions.width = static_cast<GLsizei>(width);
    root->m_dimensions.height = static_cast<GLsizei>(height);
    glViewport(0, 0, root->m_dimensions.width, root->m_dimensions.height);
}

Graphics::Window::Window(const Config& config)
    : m_title(config.title)
    , m_dimensions(config.dimensions)
    , m_cursorMode(config.cursorMode)
    , m_vSync(config.vSync) {
    if (glfwInit() != GLFW_TRUE)
        throw RF_LOCATED_ERROR("Couldn't initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_handle = glfwCreateWindow(m_dimensions.width, m_dimensions.height, m_title.c_str(), NULL, NULL);
    if (!m_handle) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't create GLFW window");
    }
    glfwMakeContextCurrent(m_handle);
    glfwSetWindowUserPointer(m_handle, this);
    glfwSetKeyCallback(m_handle, &KeyCallback);
    glfwSetMouseButtonCallback(m_handle, &MouseButtonCallback);
    glfwSetCursorPosCallback(m_handle, &CursorMoveCallback);
    glfwSetScrollCallback(m_handle, &CursorScrollCallback);
    glfwSetFramebufferSizeCallback(m_handle, &Window::FrameBufferSizeCallback);

    GLenum result = glewInit();
    if (result != GLEW_OK) {
        glfwTerminate();
        throw RF_LOCATED_ERROR("Couldn't initialize GLEW");
    }
    glViewport(0, 0, m_dimensions.width, m_dimensions.height);

    setCursorMode(m_cursorMode, false);
    setVSync(m_vSync);
}

Graphics::Window::Window(Window&& other) noexcept
    : m_handle(std::exchange(other.m_handle, nullptr))
    , m_title(std::move(other.m_title))
    , m_dimensions(std::exchange(other.m_dimensions, {}))
    , m_cursorMode(std::exchange(other.m_cursorMode, CursorMode::Normal))
    , m_vSync(std::exchange(other.m_vSync, true))
{}

Graphics::Window& Graphics::Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        m_handle = std::exchange(other.m_handle, nullptr);
        m_title = std::move(other.m_title);
        m_dimensions = std::exchange(other.m_dimensions, {});
        m_cursorMode = std::exchange(other.m_cursorMode, CursorMode::Normal);
        m_vSync = std::exchange(other.m_vSync, true);
    }
    return *this;
}

Graphics::Window::~Window() {
    if (m_handle)
        glfwTerminate();
}

void Graphics::Window::setCursorMode(CursorMode cursorMode, bool resetLastCursorPosition) {
    m_cursorMode = cursorMode;
    glfwSetInputMode(m_handle, GLFW_CURSOR, CursorModeToGlfwMacro(cursorMode));
    if (resetLastCursorPosition)
        Engine::InputMap().resetLastCursorPosition();
}

} // namespace rf
