#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace rf {

namespace Graphics {
    class Color {
    private:
        uint8_t m_red = 255;
        uint8_t m_green = 255;
        uint8_t m_blue = 255;

    public:
        Color() = default;

        Color(uint8_t red, uint8_t green, uint8_t blue);

        Color(const glm::vec3& vector);

    public:
        inline uint8_t red() const {
            return m_red;
        }

        inline uint8_t& red() {
            return m_red;
        }

        inline uint8_t green() const {
            return m_green;
        }

        inline uint8_t& green() {
            return m_green;
        }

        inline uint8_t blue() const {
            return m_blue;
        }

        inline uint8_t& blue() {
            return m_blue;
        }
    
        inline glm::vec3 vector() const {
            return { m_red / 255.0f, m_green / 255.0f, m_blue / 255.0f };
        }
    };
}

} // namespace rf
