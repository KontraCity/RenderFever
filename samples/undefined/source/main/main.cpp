#include <fmt/format.h>

#include <rf/core/error.hpp>

#include "core/game.hpp"

int main() {
    try {
        Undefined::Game game({
            .renderer = {
                .mainShaderPath = "shaders/main.rfs",
                .lightShaderPath = "shaders/light.rfs",
                .neutralShaderPath = "shaders/neutral.rfs",
            },
            .window = {
                .title = "Undefined",
                .dimensions = { 1920, 1080 },
            },
            .resourcesRootDirectory = "resources",
        });
        game.start();
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
