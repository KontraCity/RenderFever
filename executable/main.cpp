// Library {fmt}
#include <fmt/format.h>

// Library RenderFever
#include <rf/utility.hpp>

int main()
{
    fmt::print("Hello, RenderFever Engine!\n");
    fmt::print("Random integer {}\n", rf::Utility::Random(0, 100));
    fmt::print("Random real: {}\n", rf::Utility::Random(0.0, 1.0));
}
