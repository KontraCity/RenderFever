#pragma once

#include <rf/graphics/camera.hpp>
#include <rf/graphics/renderer.hpp>
#include <rf/inputs/actions.hpp>
#include <rf/inputs/input_map.hpp>

#include "binding.hpp"

namespace Game {

class Player {
private:
    rf::Camera m_camera;
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
    
public:
    Player();

private:
    float movementSpeed() const;

public:
    void reset();

public:
    const rf::Camera& camera() const {
        return m_camera;
    }
};

} // namespace Game
