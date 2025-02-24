#include <fmt/format.h>

#include <rf/graphics/window.hpp>
using namespace rf;

int main() {
    try {
        Graphics::Window window(1280, 720);
        window.run();
    }
    catch (const std::exception& error) {
        fmt::print(stderr, "Error: {}\n", error.what());
        return -1;
    }
}
