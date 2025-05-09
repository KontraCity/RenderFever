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
    Binding m_actionBinding;
    Binding m_escapeBinding;
    Entity m_cube;
    Entity m_plane;
    Player m_player;
    rf::Shader m_shader;
    rf::Renderer::UpdateDispatcher::Handle m_updateHandle;

public:
    Game();

private:
    void onUpdate(float deltaTime);

    void onAction(const rf::Event& event);

    void onEscape(const rf::Event& event);
};

} // namespace Game
