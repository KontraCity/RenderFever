#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/core/error.hpp>

#include "core/game.hpp"

int main() {
    try {
        Undefined::Game game;
        rf::Engine::Renderer().run();
        return 0;
    }
    catch (const rf::Error& error) {
        fmt::print(stderr, "Critical rf::Error!\n");
        fmt::print(stderr, "{}: {}\n", error.location(), error.message());
        if (!error.details().empty()) {
            fmt::print(stderr, "\n");
            fmt::print(stderr, "---- Details ----\n");
            fmt::print(stderr, "{}{}", error.details(), *(error.details().end() - 2) == '\n' ? "" : "\n");
            fmt::print(stderr, "-----------------\n");
        }
        return 1;
    }
}
