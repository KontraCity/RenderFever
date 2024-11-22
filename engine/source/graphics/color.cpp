#include "rf/graphics/color.hpp"

namespace rf {

Graphics::Color::Color(uint8_t red, uint8_t green, uint8_t blue) noexcept
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{}

Graphics::Color::Color(const glm::vec3& vector) noexcept
    : m_red(static_cast<uint8_t>(vector.r * 255.0))
    , m_green(static_cast<uint8_t>(vector.g * 255.0))
    , m_blue(static_cast<uint8_t>(vector.b * 255.0))
{}

} // namespace rf
