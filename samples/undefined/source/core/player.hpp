#pragma once

#include <rf/inputs/actions.hpp>
#include <rf/inputs/input_map.hpp>

#include "binding.hpp"

namespace Undefined {

class Player {
private:
    bool m_moveQuickly = false;
    bool m_moveSlowly = false;

    rf::InputMap::CursorMoveDispatcher::Handle m_cursorMoveHandle;
    rf::InputMap::CursorScrollDispatcher::Handle m_scrollHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveForwardHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveBackwardHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveLeftHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveRightHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveUpHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveDownHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveQuicklyHandle;
    rf::KeyBinding::Dispatcher::Handle m_moveSlowlyHandle;
    rf::KeyBinding::Dispatcher::Handle m_resetHandle;
    rf::KeyBinding::Dispatcher::Handle m_spawnLightHandle;
    rf::KeyBinding::Dispatcher::Handle m_toggleVSyncHandle;
    rf::KeyBinding::Dispatcher::Handle m_toggleWireframeModeHandle;
    rf::KeyBinding::Dispatcher::Handle m_switchProjectionModeHandle;
    
public:
    Player();

private:
    float movementSpeed() const;

public:
    void reset();
};

} // namespace Undefined
