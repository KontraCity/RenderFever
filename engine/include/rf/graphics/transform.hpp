#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rf {

namespace Graphics {
    class Transform {
    protected:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

    public:
        Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
            : m_position(position)
            , m_rotation(rotation)
            , m_scale(scale)
        {}

    public:
        glm::mat4 evaluateModel() const {
            // TODO: Transform around one axis rather than three
            glm::mat4 result(1.0f);
            result = glm::translate(result, m_position);
            result = glm::rotate(result, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            result = glm::rotate(result, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            result = glm::rotate(result, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            result = glm::scale(result, m_scale);
            return result;
        }

    public:
        const glm::vec3& position() const {
            return m_position;
        }

        glm::vec3& position() {
            return m_position;
        }

        const glm::vec3& rotation() const {
            return m_rotation;
        }

        glm::vec3& rotation() {
            return m_rotation;
        }

        const glm::vec3& scale() const {
            return m_scale;
        }

        glm::vec3& scale() {
            return m_scale;
        }
    };
}

} // namespace rf
