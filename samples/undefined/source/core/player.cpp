#include "player.hpp"

#include <rf/core/engine.hpp>
#include <rf/inputs/input_map.hpp>

#include "settings.hpp"
#include "utility/values.hpp"
using namespace Undefined::Settings;

namespace Undefined {

Player::Player() {
    m_cursorMoveHandle = rf::Engine::InputMap().cursorMoveDispatcher()->subscribe([this](const rf::CursorMoveAction& action) {
        // Only move the camera if the cursor is disabled!
        if (rf::Engine::Window().getCursorMode() == rf::Window::CursorMode::Disabled) {
            auto camera = rf::Engine::Scene().get<rf::Camera>();
            camera->yaw() += action.xOffset * Sensitivity::Look;
            camera->pitch() += action.yOffset / camera->zoom() * Sensitivity::Look;
        }
    });

    m_scrollHandle = rf::Engine::InputMap().scrollDispatcher()->subscribe([this](const rf::CursorScrollAction& action) {
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->zoom() += action.yOffset * Sensitivity::Zoom;
        Utility::Limit(camera->zoom(), CameraZoom::Min, CameraZoom::Max);
    });

    m_moveForwardHandle = Bind(Binding::MoveForward, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() += deltaTime * movementSpeed() * camera->evaluateDirection();
    });

    m_moveBackwardHandle = Bind(Binding::MoveBackward, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() -= deltaTime * movementSpeed() * camera->evaluateDirection();
    });

    m_moveLeftHandle = Bind(Binding::MoveLeft, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() -= deltaTime * movementSpeed() * camera->evaluateRight();
    });

    m_moveRightHandle = Bind(Binding::MoveRight, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() += deltaTime * movementSpeed() * camera->evaluateRight();
    });

    m_moveUpHandle = Bind(Binding::MoveUp, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() += deltaTime * movementSpeed() * rf::CameraConst::Up;
    });

    m_moveDownHandle = Bind(Binding::MoveDown, rf::KeyAction::Hold, [this](rf::KeyAction) {
        float deltaTime = rf::Engine::DeltaTime();
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        camera->position() -= deltaTime * movementSpeed() * rf::CameraConst::Up;
    });

    m_moveQuicklyHandle = Bind(Binding::MoveQuickly, rf::KeyAction::Press | rf::KeyAction::Release, [this](rf::KeyAction action) {
        if (action == rf::KeyAction::Press)
            m_moveQuickly = true;
        else if (action == rf::KeyAction::Release)
            m_moveQuickly = false;
    });

    m_moveSlowlyHandle = Bind(Binding::MoveSlowly, rf::KeyAction::Press | rf::KeyAction::Release, [this](rf::KeyAction action) {
        if (action == rf::KeyAction::Press)
            m_moveSlowly = true;
        else if (action == rf::KeyAction::Release)
            m_moveSlowly = false;
    });

    m_resetHandle = Bind(Binding::ResetPlayer, rf::KeyAction::Press | rf::KeyAction::Repeat, [this](rf::KeyAction action) {
        if (action == rf::KeyAction::Press)
            rf::Engine::Scene().get<rf::Camera>()->zoom() = 1.0f;
        else if (action == rf::KeyAction::Repeat)
            reset();
    });
    reset();

    m_toggleVSyncHandle = Bind(Binding::ToggleVSync, rf::KeyAction::Press, [this](rf::KeyAction) {
        rf::Window& window = rf::Engine::Window();
        window.setVSync(!window.getVSync());
    });

    m_toggleWireframeModeHandle = Bind(Binding::ToggleWireframeMode, rf::KeyAction::Press, [this](rf::KeyAction) {
        rf::Window& window = rf::Engine::Window();
        window.setWireframeMode(!window.getWireframeMode());
    });

    m_switchProjectionModeHandle = Bind(Binding::SwitchProjectionMode, rf::KeyAction::Press, [this](rf::KeyAction) {
        auto camera = rf::Engine::Scene().get<rf::Camera>();
        switch (camera->projectionMode()) {
            case rf::ProjectionMode::Perspective:
                camera->projectionMode() = rf::ProjectionMode::Orthographic;
                return;
            case rf::ProjectionMode::Orthographic:
                camera->projectionMode() = rf::ProjectionMode::Perspective;
                return;
        }
    });
}

float Player::movementSpeed() const {
    if (!m_moveSlowly && m_moveQuickly)
        return MovementSpeed::Quick;
    else if (m_moveSlowly && !m_moveQuickly)
        return MovementSpeed::Slow;
    return MovementSpeed::Normal;
}

void Player::reset() {
    auto camera = rf::Engine::Scene().get<rf::Camera>();
    camera->position() = { -10.0f, 10.0f, -10.0f };
    camera->yaw() = 135.0f;
    camera->pitch() = -37.5f;
    camera->zoom() = 1.0f;
}

} // namespace Undefined
