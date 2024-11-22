#pragma once

// Graphics libraries
#include <glm/glm.hpp>

// Custom modules
#include "rf/graphics/lighting/types.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics
{
    namespace Lighting
    {
        class DirectionalLight
        {
        private:
            Properties m_properties;
            Color m_color;
            glm::vec3 m_direction;

        public:
            /// @brief Illuminate scene
            /// @param shader The shader to illuminate with
            void illuminate(Shader& shader) const;

        public:
            /// @brief Get light properties
            /// @return Light properties
            inline const Properties& properties() const
            {
                return m_properties;
            }

            /// @brief Get light properties
            /// @return Light properties
            inline Properties& properties()
            {
                return m_properties;
            }

            /// @brief Get light color
            /// @return Light color
            inline Color color() const
            {
                return m_color;
            }

            /// @brief Get light color
            /// @return Light color
            inline Color& color()
            {
                return m_color;
            }

            /// @brief Get light direction
            /// @return Light direction
            inline const glm::vec3& direction() const
            {
                return m_direction;
            }

            /// @brief Get light direction
            /// @return Light direction
            inline glm::vec3& direction()
            {
                return m_direction;
            }
        };
    }
}

} // namespace rf
