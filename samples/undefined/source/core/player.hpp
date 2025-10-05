#pragma once

#include <rf/input/input_map.hpp>
#include <rf/input/key_actions.hpp>

#include "binding.hpp"

namespace Undefined {

class Player {
private:
    bool m_moveQuickly = false;
    bool m_moveSlowly = false;

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
    
public:
    Player();

private:
    float movementSpeed() const;

public:
    void reset();
};

} // namespace Undefined
