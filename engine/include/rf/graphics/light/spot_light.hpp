#pragma once

#include <glm/glm.hpp>

#include "rf/graphics/light/types.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    namespace Light {
        class SpotLight {
        private:
            Properties m_properties;
            Attenuation m_attenuation;
            Cutoff m_cutoff;
            Color m_color;
            glm::vec3 m_position;
            glm::vec3 m_direction;

        public:
            void illuminate(Shader& shader) const;

        public:
            inline const Properties& properties() const {
                return m_properties;
            }

            inline Properties& properties() {
                return m_properties;
            }

            inline const Attenuation& attenuation() const {
                return m_attenuation;
            }

            inline Attenuation& attenuation() {
                return m_attenuation;
            }

            inline Cutoff cutoff() const {
                return m_cutoff;
            }

            inline Cutoff& cutoff() {
                return m_cutoff;
            }
        
            inline Color color() const {
                return m_color;
            }

            inline Color& color() {
                return m_color;
            }

            inline const glm::vec3& position() const {
                return m_position;
            }

            inline glm::vec3& position() {
                return m_position;
            }

            inline const glm::vec3& direction() const {
                return m_direction;
            }

            inline glm::vec3& direction() {
                return m_direction;
            }
        };
    }
}

} // namespace rf
