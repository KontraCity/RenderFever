#pragma once

#include <rf/core/engine.hpp>
#include <rf/input/input_map.hpp>
#include <rf/input/key_actions.hpp>
#include <rf/world/entity.hpp>

#include "binding.hpp"

namespace Undefined {

class Game {
private:
    // TODO: Make axis bindings!
    // rf::Input::AxisBinding::AxisHandle
    rf::Input::InputMap::CursorMoveDispatcher::Handle m_cursorMoveHandle;
    rf::Input::InputMap::CursorScrollDispatcher::Handle m_cursorScrollHandle;
    rf::Input::KeyBindingCallbackHandle m_moveForwardHandle;
    rf::Input::KeyBindingCallbackHandle m_moveBackwardHandle;
    rf::Input::KeyBindingCallbackHandle m_moveLeftHandle;
    rf::Input::KeyBindingCallbackHandle m_moveRightHandle;
    rf::Input::KeyBindingCallbackHandle m_moveUpHandle;
    rf::Input::KeyBindingCallbackHandle m_moveDownHandle;
    rf::Input::KeyBindingCallbackHandle m_moveQuicklyHandle;
    rf::Input::KeyBindingCallbackHandle m_moveSlowlyHandle;
    rf::Input::KeyBindingCallbackHandle m_resetHandle;
    rf::Input::KeyBindingCallbackHandle m_spawnLightHandle;
    rf::Input::KeyBindingCallbackHandle m_toggleVSyncHandle;
    rf::Input::KeyBindingCallbackHandle m_toggleWireframeModeHandle;
    rf::Input::KeyBindingCallbackHandle m_switchProjectionModeHandle;
    rf::Input::KeyBindingCallbackHandle m_switchToStandaloneCameraHandle;

    bool m_moveQuickly = false;
    bool m_moveSlowly = false;

    rf::World::EntityId m_playerEntityId = 0;
    rf::World::EntityId m_standaloneCameraEntityId = 0;

public:
    Game(const rf::Engine::Config& config);

    Game(const Game& other) = delete;

    Game(Game&& other) = delete;

    ~Game();

public:
    Game& operator=(const Game& other) = delete;

    Game& operator=(Game&& other) = delete;

private:
    void handlesSetup();

    void sceneSetup();

    float movementSpeed() const;

    void resetCamera() const;

public:
    void start() const;
};

} // namespace Undefined
