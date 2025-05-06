#include "player.hpp"

#include <rf/core/engine.hpp>
#include <rf/inputs/map.hpp>

#include "settings.hpp"
#include "utility.hpp"
using namespace Game::Settings;

namespace Game {

Player::Player()
    : m_updateHandle(rf::Engine::Renderer().updateDispatcher()->subscribe(std::bind(&Player::onUpdate, this, _1)))
    , m_cursorMoveBinding(Binding::Type::CursorMove, std::bind(&Player::onCursorMove, this, _1))
    , m_scrollBinding(Binding::Type::Scroll, std::bind(&Player::onScroll, this, _1))
    , m_resetBinding(Binding::Type::ResetPlayer, std::bind(&Player::onReset, this, _1)) {
    reset();
}

void Player::onCursorMove(const rf::Inputs::Event& event) {
    if (event.type() != rf::Inputs::Event::Type::CursorMoveEvent)
        return;
    const auto& cursorMoveEvent = std::get<rf::Inputs::CursorMoveEvent>(event);

    float x = cursorMoveEvent.xPosition;
    float y = cursorMoveEvent.yPosition;
    static float lastX = x, lastY = y;
    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x; lastY = y;

    m_camera.yaw() += xOffset * Sensitivity::Look;
    m_camera.pitch()  += yOffset / m_camera.zoom() * Sensitivity::Look;
    Utility::Limit(m_camera.pitch(), -89.9f, 89.9f);
}

void Player::onScroll(const rf::Inputs::Event& event) {
    if (event.type() != rf::Inputs::Event::Type::ScrollEvent)
        return;
    const auto& scrollEvent = std::get<rf::Inputs::ScrollEvent>(event);

    m_camera.zoom() += scrollEvent.yOffset * Sensitivity::Zoom;
    Utility::Limit(m_camera.zoom(), CameraZoom::Min, CameraZoom::Max);
}

void Player::onUpdate(float deltaTime) {
    bool moveSlowly = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveSlowly));
    bool moveQuickly = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveQuickly));
    float speed = 0.0f;
    if (moveSlowly && !moveQuickly)
        speed = Settings::MovementSpeed::Slow;
    else if (!moveSlowly && moveQuickly)
        speed = Settings::MovementSpeed::Quick;
    else
        speed = Settings::MovementSpeed::Normal;

    bool moveBackward = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveBackward));
    bool moveForward = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveForward));
    glm::vec3 direction = m_camera.evaluateDirection();
    if (!moveBackward && moveForward)
        m_camera.position() += deltaTime * speed * direction;
    else if (moveBackward && !moveForward)
        m_camera.position() -= deltaTime * speed * direction;

    bool moveLeft = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveLeft));
    bool moveRight = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveRight));
    if (!moveLeft && moveRight)
        m_camera.position() += deltaTime * speed * glm::normalize(glm::cross(direction, rf::Graphics::CameraConst::Up));
    else if (moveLeft && !moveRight)
        m_camera.position() -= deltaTime * speed * glm::normalize(glm::cross(direction, rf::Graphics::CameraConst::Up));

    bool moveDown = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveDown));
    bool moveUp = rf::Inputs::KeyPressed(Binding::TypeToInput(Binding::Type::MoveUp));
    if (!moveDown && moveUp)
        m_camera.position() += deltaTime * speed * rf::Graphics::CameraConst::Up;
    else if (moveDown && !moveUp)
        m_camera.position() -= deltaTime * speed * rf::Graphics::CameraConst::Up;
}

void Player::onReset(const rf::Inputs::Event& event) {
    if (event.type() != rf::Inputs::Event::Type::KeyEvent)
        return;
    const auto& keyEvent = std::get<rf::Inputs::KeyEvent>(event);
    
    if (keyEvent == rf::Inputs::KeyEvent::Press)
        reset();
}

void Player::reset() {
    m_camera.position() = { -3.0f, 0.0f, 0.0f };
    m_camera.yaw() = 0.0f;
    m_camera.pitch() = 0.0f;
    m_camera.zoom() = 1.0f;
}

} // namespace Game
