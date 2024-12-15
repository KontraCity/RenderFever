#include "rf/graphics/camera.hpp"
using namespace rf::Graphics::CameraConst;

namespace rf {

Graphics::Camera::Camera()
{
    reset();
    m_keyEventSubscriberId = Engine::Input::Key.subscribe(std::bind(&Camera::onKey, this, _1, _2, _3));
    m_inputEventSubscriberId = Engine::Input::Input.subscribe(std::bind(&Camera::onInput, this, _1, _2));
    m_cursorPosEventSubscriberId = Engine::Input::CursorPos.subscribe(std::bind(&Camera::onCursorPos, this, _1, _2));
    m_scrollEventSubscriberId = Engine::Input::Scroll.subscribe(std::bind(&Camera::onScroll, this, _1, _2));
}

Graphics::Camera::~Camera()
{
    Engine::Input::Key.unsubscribe(m_keyEventSubscriberId);
    Engine::Input::Input.unsubscribe(m_inputEventSubscriberId);
    Engine::Input::CursorPos.unsubscribe(m_cursorPosEventSubscriberId);
    Engine::Input::Scroll.unsubscribe(m_scrollEventSubscriberId);
}

void Graphics::Camera::onKey(int key, int action, int mods)
{
    if (key == GLFW_KEY_R)
    {
        if (action == GLFW_PRESS)
            m_zoom = 1.0f;
        else if (action == GLFW_REPEAT)
            reset();
    }
}

void Graphics::Camera::onInput(GLFWwindow* window, float deltaTime)
{
    // Movement speed
    bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    bool alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    float speed = Speed::Normal;
    if (shift && !alt)
        speed = Speed::Fast;
    else if (!shift && alt)
        speed = Speed::Slow;

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        m_position.y += deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        m_position.y -= deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_W))
        m_position += deltaTime * speed * m_direction;
    if (glfwGetKey(window, GLFW_KEY_S))
        m_position -= deltaTime * speed * m_direction;
    if (glfwGetKey(window, GLFW_KEY_D))
        m_position += deltaTime * speed * glm::normalize(glm::cross(m_direction, m_up));
    if (glfwGetKey(window, GLFW_KEY_A))
        m_position -= deltaTime * speed * glm::normalize(glm::cross(m_direction, m_up));
}

void Graphics::Camera::onCursorPos(double x, double y)
{
    static double lastX = x, lastY = y;
    double xOffset = x - lastX;
    double yOffset = lastY - y;
    lastX = x; lastY = y;

    m_yaw += xOffset * Sensivity::Move;
    m_pitch = Engine::Utility::Limit(m_pitch + yOffset * Sensivity::Move / m_zoom, Pitch::Min, Pitch::Max);
}

void Graphics::Camera::onScroll(double xOffset, double yOffset)
{
    m_zoom = Engine::Utility::Limit(m_zoom + yOffset * Sensivity::Scroll * m_zoom, Zoom::Min, Zoom::Max);
}

void Graphics::Camera::capture(Shader& shader, Shader& lightingShader, Shader& skyboxShader, unsigned int width, unsigned int height)
{
    // Direction
    glm::vec3 direction(
        std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
        std::sin(glm::radians(m_pitch)),
        std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))
    );
    m_direction = glm::normalize(direction);

    // View
    glm::mat4 view = glm::lookAt(m_position, m_position + m_direction, m_up);
    shader.set("View", view);
    lightingShader.set("View", view);
    skyboxShader.set("View", glm::mat3(view)); // to remove translation

    // Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f / m_zoom), static_cast<float>(width) / height, Perspective::Near, Perspective::Far);
    shader.set("Projection", projection);
    lightingShader.set("Projection", projection);
    skyboxShader.set("Projection", projection);
}

void Graphics::Camera::reset()
{
    m_position = Defaults::Position;
    m_direction = Defaults::Direction;
    m_up = Defaults::Up;
    m_yaw = Defaults::Yaw;
    m_pitch = Defaults::Pitch;
    m_zoom = Defaults::Zoom;
}

} // namespace rf
 