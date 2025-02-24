#include <fmt/format.h>

#include <rf/core/error.hpp>
#include <rf/graphics/window.hpp>
using namespace rf;

int main() {
    try {
        Graphics::Window window(1280, 720);
        window.run();
    }
    catch (const Error& error) {
        fmt::print(stderr, "{}: {}\n", error.location(), error.message());
        if (!error.details().empty())
            fmt::print("Details:\n{}\n", error.details());
        return -1;
    }
}
