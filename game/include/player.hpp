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
    rf::Graphics::Camera m_camera;
    rf::Graphics::Renderer::UpdateDispatcher::Handle m_updateHandle;

public:
    Player();

private:
    void onUpdate(float deltaTime);

    void onCursorMove(const rf::Inputs::Event& event);

    void onScroll(const rf::Inputs::Event& event);

    void onReset(const rf::Inputs::Event& event);

public:
    void reset();

public:
    const rf::Graphics::Camera& camera() const {
        return m_camera;
    }

    rf::Graphics::Camera& camera() {
        return m_camera;
    }
};

} // namespace Game
