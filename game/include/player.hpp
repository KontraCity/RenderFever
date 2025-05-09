#pragma once

#include <rf/graphics/camera.hpp>
#include <rf/graphics/renderer.hpp>
#include <rf/inputs/event.hpp>

#include "binding.hpp"

namespace Game {

class Player {
private:
    Binding m_cursorMoveBinding;
    Binding m_scrollBinding;
    Binding m_resetBinding;
    rf::Camera m_camera;
    rf::Renderer::UpdateDispatcher::Handle m_updateHandle;

public:
    Player();

private:
    void onUpdate(float deltaTime);

    void onCursorMove(const rf::Event& event);

    void onScroll(const rf::Event& event);

    void onReset(const rf::Event& event);

public:
    void reset();

public:
    const rf::Camera& camera() const {
        return m_camera;
    }

    rf::Camera& camera() {
        return m_camera;
    }
};

} // namespace Game
