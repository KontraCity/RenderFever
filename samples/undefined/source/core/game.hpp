#pragma once

#include "binding.hpp"
#include "player.hpp"

namespace Undefined {

class Game {
private:
    // TODO: The player should be a part of the ECS system!
    Player m_player;

public:
    Game();

    void start() const;
};

} // namespace Undefined
