#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    class Transform {
    protected:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

    public:
        Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

    public:
        void apply(Shader& shader) const;

    public:
        inline const glm::vec3& position() const {
            return m_position;
        }

        inline glm::vec3& position() {
            return m_position;
        }

        inline const glm::vec3& rotation() const {
            return m_rotation;
        }

        inline glm::vec3& rotation() {
            return m_rotation;
        }

        inline const glm::vec3& scale() const {
            return m_scale;
        }

        inline glm::vec3& scale() {
            return m_scale;
        }
    };
}

} // namespace rf
