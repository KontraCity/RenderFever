#pragma once

#include <rf/graphics/renderer.hpp>
#include <rf/graphics/shader.hpp>
#include <rf/inputs/event.hpp>

#include "binding.hpp"
#include "entity.hpp"
#include "player.hpp"

namespace Game {

class Game {
private:
    Binding m_closeWindowBinding;
    Entity m_cube;
    Entity m_plane;
    Player m_player;
    rf::Graphics::Shader m_shader;
    rf::Graphics::Renderer::UpdateDispatcher::Handle m_updateHandle;

public:
    Game();

private:
    void onUpdate(float deltaTime);

    void onCloseWindow(const rf::Inputs::Event& event);
};

} // namespace Game
