// Library {fmt}
#include <fmt/format.h>

// RenderFever Engine
#include <rf/graphics/window.hpp>
using namespace rf;

int main()
{
    try
    {
        Graphics::Window window(1280, 720);
        window.run();
    }
    catch (const std::exception& error)
    {
        fmt::print(stderr, "Error: {}\n", error.what());
        return -1;
    }
    catch (...)
    {
        fmt::print(stderr, "Unknown error\n");
        return -1;
    }
}
