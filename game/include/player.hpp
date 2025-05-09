#pragma once

#include <rf/graphics/camera.hpp>
#include <rf/graphics/renderer.hpp>
#include <rf/inputs/actions.hpp>

#include "binding.hpp"

namespace Game {

class Player {
private:
    rf::Camera m_camera;
    bool m_moveQuickly = false;
    bool m_moveSlowly = false;

    rf::CursorMoveBinding::Dispatcher::Handle m_cursorMoveBinding;
    rf::ScrollBinding::Dispatcher::Handle m_scrollBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveForwardBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveBackwardBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveLeftBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveRightBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveUpBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveDownBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveQuicklyBinding;
    rf::KeyBinding::Dispatcher::Handle m_moveSlowlyBinding;
    rf::KeyBinding::Dispatcher::Handle m_resetBinding;
    

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
