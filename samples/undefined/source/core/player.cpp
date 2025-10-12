#include "player.hpp"

#include <rf/core/engine.hpp>
#include <rf/core/math.hpp>

#include "settings.hpp"
#include "utility/values.hpp"
using namespace Undefined::Settings;

namespace Undefined {

Player::Player() {
    m_cursorMoveHandle = rf::Engine::InputMap().cursorMoveDispatcher()->subscribe([this](const rf::Input::CursorMoveAction& action) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->yaw += action.xOffset * Sensitivity::Look;
        camera->pitch += action.yOffset / camera->zoom * Sensitivity::Look;
    });

    m_cursorScrollHandle = rf::Engine::InputMap().cursorScrollDispatcher()->subscribe([this](const rf::Input::CursorScrollAction& action) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->zoom += action.yOffset * Sensitivity::Zoom;
        Utility::Limit(camera->zoom, CameraZoom::Min, CameraZoom::Max);
    });

    m_moveForwardHandle = Bind(Binding::MoveForward, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position += deltaTime * movementSpeed() * rf::Math::EvaluateDirection(*camera.get());
    });

    m_moveBackwardHandle = Bind(Binding::MoveBackward, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position -= deltaTime * movementSpeed() * rf::Math::EvaluateDirection(*camera.get());
    });

    m_moveLeftHandle = Bind(Binding::MoveLeft, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position -= deltaTime * movementSpeed() * rf::Math::EvaluateRight(*camera.get());
    });

    m_moveRightHandle = Bind(Binding::MoveRight, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position += deltaTime * movementSpeed() * rf::Math::EvaluateRight(*camera.get());
    });

    m_moveUpHandle = Bind(Binding::MoveUp, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position += deltaTime * movementSpeed() * rf::Graphics::Camera::Up;
    });

    m_moveDownHandle = Bind(Binding::MoveDown, rf::Input::KeyAction::Hold, [this](rf::Input::KeyAction, float deltaTime) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        camera->position -= deltaTime * movementSpeed() * rf::Graphics::Camera::Up;
    });

    m_moveQuicklyHandle = Bind(Binding::MoveQuickly, rf::Input::KeyAction::Press | rf::Input::KeyAction::Release, [this](rf::Input::KeyAction action, float) {
        if (action == rf::Input::KeyAction::Press)
            m_moveQuickly = true;
        else if (action == rf::Input::KeyAction::Release)
            m_moveQuickly = false;
    });

    m_moveSlowlyHandle = Bind(Binding::MoveSlowly, rf::Input::KeyAction::Press | rf::Input::KeyAction::Release, [this](rf::Input::KeyAction action, float) {
        if (action == rf::Input::KeyAction::Press)
            m_moveSlowly = true;
        else if (action == rf::Input::KeyAction::Release)
            m_moveSlowly = false;
    });

    m_resetHandle = Bind(Binding::ResetPlayer, rf::Input::KeyAction::Press | rf::Input::KeyAction::Repeat, [this](rf::Input::KeyAction action, float) {
        if (action == rf::Input::KeyAction::Press)
            rf::Engine::Scene().get<rf::Graphics::Camera>()->zoom = 1.0f;
        else if (action == rf::Input::KeyAction::Repeat)
            reset();
    });
    reset();

    m_spawnLightHandle = Bind(Binding::SpawnLight, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::World::Scene& scene = rf::Engine::Scene();
        auto camera = scene.get<rf::Graphics::Camera>();

        rf::World::Entity& spawnedLight = scene.newEntity("Spawned Light");
        spawnedLight.setComponent<rf::World::DrawComponent>({
            .transform = {
                .position = camera->position,
                .rotation = { camera->pitch, -camera->yaw, 0.0f },
                .scale = { 0.1f, 0.1f, 0.4f },
            },
            .material = {
                .shader = rf::Engine::Library().loadShader("light/"),
            },
            .mesh = rf::Engine::Library().loadMesh("cube.glb"),
        });
        spawnedLight.setComponent<rf::World::LightComponent>({
            .light = {
                .type = rf::Graphics::LightType::SpotLight,
                .position = camera->position,
                .direction = rf::Math::EvaluateDirection(*camera.get()),
                .spotInnerCutoff = 7.0f,
                .spotOuterCutoff = 10.0f,
            }
        });
    });

    m_toggleVSyncHandle = Bind(Binding::ToggleVSync, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::Graphics::Window& window = rf::Engine::Window();
        window.setVSync(!window.getVSync());
    });

    m_toggleWireframeModeHandle = Bind(Binding::ToggleWireframeMode, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        rf::Graphics::Renderer& renderer = rf::Engine::Renderer();
        renderer.setWireframeMode(!renderer.getWireframeMode());
    });

    m_switchProjectionModeHandle = Bind(Binding::SwitchProjectionMode, rf::Input::KeyAction::Press, [this](rf::Input::KeyAction, float) {
        auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
        if (camera->projectionMode == rf::Graphics::ProjectionMode::Perspective)
            camera->projectionMode = rf::Graphics::ProjectionMode::Orthographic;
        else
            camera->projectionMode = rf::Graphics::ProjectionMode::Perspective;
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
    auto camera = rf::Engine::Scene().get<rf::Graphics::Camera>();
    camera->position = { -2.0f, 2.0f, 2.0f };
    camera->yaw      = 45.0f;
    camera->pitch    = -37.5f;
    camera->zoom     = 1.0f;
}

} // namespace Undefined
