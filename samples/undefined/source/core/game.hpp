#pragma once

#include <rf/graphics/renderer.hpp>
#include <rf/graphics/shader.hpp>

#include "binding.hpp"
#include "player.hpp"

namespace Undefined {

class Game {
private:
    // TODO: The player should be a part of the ECS system!
    Player m_player;
    rf::KeyBinding::Dispatcher::Handle m_actionHandle;
    rf::KeyBinding::Dispatcher::Handle m_escapeHandle;

public:
    Game();

    void start() const;
};

} // namespace Undefined
