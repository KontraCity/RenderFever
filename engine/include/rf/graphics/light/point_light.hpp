#pragma once

#include <glm/glm.hpp>

#include "rf/graphics/light/types.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    namespace Light {
        class PointLight {
        private:
            Properties m_properties;
            Attenuation m_attenuation;
            Color m_color;
            glm::vec3 m_position;

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
        };
    }
}

} // namespace rf
