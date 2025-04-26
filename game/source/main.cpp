#include <fmt/format.h>

#include <rf/core/error.hpp>

#include "game.hpp"

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const rf::Error& error) {
        fmt::print(stderr, "Critical rf::Error!\n");
        fmt::print(stderr, "{}\n", error.what());
        return 1;
    }
}
