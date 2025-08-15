#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace rf {

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
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_rotation)));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
        return translation * rotation * scale;
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

} // namespace rf
