#pragma once

#include <glm/glm.hpp>

#include "rf/graphics/light/types.hpp"
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    namespace Light {
        class DirectionalLight {
        private:
            Properties m_properties;
            Color m_color;
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

            inline Color color() const {
                return m_color;
            }

            inline Color& color() {
                return m_color;
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
