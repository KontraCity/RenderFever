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
        class SpotLight
        {
        private:
            Properties m_properties;
            Attenuation m_attenuation;
            Cutoff m_cutoff;
            Color m_color;
            glm::vec3 m_position;
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

            /// @brief Get light cutoff
            /// @return Light cutoff
            inline Cutoff cutoff() const
            {
                return m_cutoff;
            }

            /// @brief Get light cutoff
            /// @return Light cutoff
            inline Cutoff& cutoff()
            {
                return m_cutoff;
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
