#pragma once

#include <rf/graphics/renderer.hpp>
#include <rf/graphics/shader.hpp>

#include "binding.hpp"
#include "entity.hpp"
#include "player.hpp"

namespace Undefined {

class Game {
private:
    rf::Shader m_shader;
    Entity m_cube;
    Entity m_plane;
    Player m_player;

    rf::KeyBinding::Dispatcher::Handle m_actionHandle;
    rf::KeyBinding::Dispatcher::Handle m_escapeHandle;
    rf::Renderer::UpdateDispatcher::Handle m_updateHandle;

public:
    Game();
};

} // namespace Undefined
