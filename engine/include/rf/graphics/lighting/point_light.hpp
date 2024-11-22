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
        class PointLight
        {
        private:
            Properties m_properties;
            Attenuation m_attenuation;
            Color m_color;
            glm::vec3 m_position;

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

            /// @brief Get light attenuation
            /// @return Light attenuation
            inline const Attenuation& attenuation() const
            {
                return m_attenuation;
            }

            /// @brief Get light attenuation
            /// @return Light attenuation
            inline Attenuation& attenuation()
            {
                return m_attenuation;
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

            /// @brief Get light position
            /// @return Light position
            inline const glm::vec3& position() const
            {
                return m_position;
            }

            /// @brief Get light position
            /// @return Light position
            inline glm::vec3& position()
            {
                return m_position;
            }
        };
    }
}

} // namespace rf
