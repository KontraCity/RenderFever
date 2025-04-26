#include <fmt/format.h>

#include <rf/core/error.hpp>
#include <rf/core/engine.hpp>
using namespace rf;

int main() {
    try {
        Engine::Window().rename("Game");
        Engine::Window().resize({ 1920, 1080 });
        while (true) Engine::Window().update();
    }
    catch (const Error& error) {
        fmt::print(stderr, "Critical rf::Error!\n");
        fmt::print(stderr, "{}\n", error.what());
        return 1;
    }
}
