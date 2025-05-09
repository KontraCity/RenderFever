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

void Player::onCursorMove(const rf::Event& event) {
    if (event.type() != rf::Event::Type::CursorMoveEvent)
        return;
    const auto& cursorMoveEvent = std::get<rf::CursorMoveEvent>(event);

    // Don't move the camera if cursor is not disabled!
    if (rf::Engine::Window().cursorMode() != rf::Window::CursorMode::Disabled)
        return;

    m_camera.yaw() += cursorMoveEvent.xOffset * Sensitivity::Look;
    m_camera.pitch()  += cursorMoveEvent.yOffset / m_camera.zoom() * Sensitivity::Look;
    Utility::Limit(m_camera.pitch(), -89.9f, 89.9f);
}

void Player::onScroll(const rf::Event& event) {
    if (event.type() != rf::Event::Type::ScrollEvent)
        return;
    const auto& scrollEvent = std::get<rf::ScrollEvent>(event);

    m_camera.zoom() += scrollEvent.yOffset * Sensitivity::Zoom;
    Utility::Limit(m_camera.zoom(), CameraZoom::Min, CameraZoom::Max);
}

void Player::onUpdate(float deltaTime) {
    bool moveSlowly = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveSlowly));
    bool moveQuickly = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveQuickly));
    float speed = 0.0f;
    if (moveSlowly && !moveQuickly)
        speed = Settings::MovementSpeed::Slow;
    else if (!moveSlowly && moveQuickly)
        speed = Settings::MovementSpeed::Quick;
    else
        speed = Settings::MovementSpeed::Normal;

    bool moveBackward = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveBackward));
    bool moveForward = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveForward));
    glm::vec3 direction = m_camera.evaluateDirection();
    if (!moveBackward && moveForward)
        m_camera.position() += deltaTime * speed * direction;
    else if (moveBackward && !moveForward)
        m_camera.position() -= deltaTime * speed * direction;

    bool moveLeft = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveLeft));
    bool moveRight = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveRight));
    if (!moveLeft && moveRight)
        m_camera.position() += deltaTime * speed * m_camera.evaluateRight();
    else if (moveLeft && !moveRight)
        m_camera.position() -= deltaTime * speed * m_camera.evaluateRight();

    bool moveDown = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveDown));
    bool moveUp = rf::KeyPressed(Binding::TypeToInput(Binding::Type::MoveUp));
    if (!moveDown && moveUp)
        m_camera.position() += deltaTime * speed * rf::CameraConst::Up;
    else if (moveDown && !moveUp)
        m_camera.position() -= deltaTime * speed * rf::CameraConst::Up;
}

void Player::onReset(const rf::Event& event) {
    if (event.type() != rf::Event::Type::KeyEvent)
        return;
    const auto& keyEvent = std::get<rf::KeyEvent>(event);
    
    if (keyEvent == rf::KeyEvent::Press)
        reset();
}

void Player::reset() {
    m_camera.position() = { -3.0f, 0.0f, 0.0f };
    m_camera.yaw() = 0.0f;
    m_camera.pitch() = 0.0f;
    m_camera.zoom() = 1.0f;
}

} // namespace Game
