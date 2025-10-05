#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/core/error.hpp>

#include "core/game.hpp"

static void Run() {
    rf::Engine::Init({
        .renderer = {
            .mainShaderPath = "main",
            .lightShaderPath = "light",
        },
        .window = {
            .title = "Undefined",
            .dimensions = { 1920, 1080 },
        },
        .library = {
            .resourcesPath = "resources",
            .shadersPath = "shaders",
            .texturesPath = "textures",
        },
    });

    Undefined::Game game;
    game.start();

    rf::Engine::Shutdown();
}

int main() {
    try {
        Run();
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
