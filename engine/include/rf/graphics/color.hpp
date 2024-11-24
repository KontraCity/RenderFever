#pragma once

// STL modules
#include <cstdint>

// Graphics libraries
#include <glm/glm.hpp>

namespace rf {

namespace Graphics
{
    class Color
    {
    private:
        uint8_t m_red = 255;
        uint8_t m_green = 255;
        uint8_t m_blue = 255;

    public:
        Color() = default;

        /// @brief Create color from components
        /// @param red Red color component
        /// @param green Green color component
        /// @param blue Blue color component
        Color(uint8_t red, uint8_t green, uint8_t blue);

        /// @brief Create color from color vector
        /// @param vector Color vector
        Color(const glm::vec3& vector);

    public:
        /// @brief Get red color component
        /// @return Red color component
        inline uint8_t red() const
        {
            return m_red;
        }

        /// @brief Get red color component
        /// @return Red color component
        inline uint8_t& red()
        {
            return m_red;
        }

        /// @brief Get green color component
        /// @return Green color component
        inline uint8_t green() const
        {
            return m_green;
        }

        /// @brief Get green color component
        /// @return Green color component
        inline uint8_t& green()
        {
            return m_green;
        }

        /// @brief Get blue color component
        /// @return Blue color component
        inline uint8_t blue() const
        {
            return m_blue;
        }

        /// @brief Get blue color component
        /// @return Blue color component
        inline uint8_t& blue()
        {
            return m_blue;
        }
    
        /// @brief Get color vector
        /// @return Color vector
        inline glm::vec3 vector() const
        {
            return { m_red / 255.0f, m_green / 255.0f, m_blue / 255.0f };
        }
    };
}

} // namespace rf
