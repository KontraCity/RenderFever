#include "player.hpp"

#include <rf/core/engine.hpp>
#include <rf/inputs/input_map.hpp>

#include "settings.hpp"
#include "utility.hpp"
using namespace Game::Settings;

namespace Game {

Player::Player() {
    m_cursorMoveBinding = rf::Engine::InputMap().cursorMoveBinding().dispatcher->subscribe([this](const rf::CursorMoveAction& action) {
        // Only move the camera if the cursor is disabled!
        if (rf::Engine::Window().cursorMode() == rf::Window::CursorMode::Disabled) {
            m_camera.yaw() += action.xOffset * Sensitivity::Look;
            m_camera.pitch() += action.yOffset / m_camera.zoom() * Sensitivity::Look;
            Utility::Limit(m_camera.pitch(), -89.9f, 89.9f);
        }
    });

    m_scrollBinding = rf::Engine::InputMap().scrollBinding().dispatcher->subscribe([this](const rf::ScrollAction& action) {
        m_camera.zoom() += action.yOffset * Sensitivity::Zoom;
        Utility::Limit(m_camera.zoom(), CameraZoom::Min, CameraZoom::Max);
    });

    m_moveForwardBinding = Bind(Binding::MoveForward, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() += deltaTime * movementSpeed() * m_camera.evaluateDirection(); 
    });

    m_moveBackwardBinding = Bind(Binding::MoveBackward, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() -= deltaTime * movementSpeed() * m_camera.evaluateDirection();
    });

    m_moveLeftBinding = Bind(Binding::MoveLeft, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() -= deltaTime * movementSpeed() * m_camera.evaluateRight();
    });

    m_moveRightBinding = Bind(Binding::MoveRight, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() += deltaTime * movementSpeed() * m_camera.evaluateRight();
    });

    m_moveUpBinding = Bind(Binding::MoveUp, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() += deltaTime * movementSpeed() * rf::CameraConst::Up;
    });

    m_moveDownBinding = Bind(Binding::MoveDown, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::Renderer().deltaTime();
        m_camera.position() -= deltaTime * movementSpeed() * rf::CameraConst::Up;
    });

    m_moveQuicklyBinding = Bind(Binding::MoveQuickly, rf::KeyAction::Press | rf::KeyAction::Release, [this](rf::KeyAction action) {
        if (action == rf::KeyAction::Press)
            m_moveQuickly = true;
        else if (action == rf::KeyAction::Release)
            m_moveQuickly = false;
    });

    m_moveSlowlyBinding = Bind(Binding::MoveSlowly, rf::KeyAction::Press | rf::KeyAction::Release, [this](rf::KeyAction action) {
        if (action == rf::KeyAction::Press)
            m_moveSlowly = true;
        else if (action == rf::KeyAction::Release)
            m_moveSlowly = false;
    });

    m_resetBinding = Bind(Binding::ResetPlayer, rf::KeyAction::Press, std::bind(&Player::reset, this));
    reset();
}

float Player::movementSpeed() const {
    if (!m_moveSlowly && m_moveQuickly)
        return MovementSpeed::Quick;
    else if (m_moveSlowly && !m_moveQuickly)
        return MovementSpeed::Slow;
    return MovementSpeed::Normal;
}

void Player::reset() {
    m_camera.position() = { -3.0f, 0.0f, 0.0f };
    m_camera.yaw() = 0.0f;
    m_camera.pitch() = 0.0f;
    m_camera.zoom() = 1.0f;
}

} // namespace Game
