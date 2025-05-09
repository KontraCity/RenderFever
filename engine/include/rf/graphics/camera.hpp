#pragma once

#include <cmath>

#include <glm/glm.hpp>

namespace rf {

namespace CameraConst {
    constexpr glm::vec3 Up(0.0f, 1.0f, 0.0f);
    constexpr float NearPlane = 0.1f;
    constexpr float FarPlane = 10000.0f;
}

class Camera {
protected:
    glm::vec3 m_position;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_zoom = 1.0f;

public:
    Camera(const glm::vec3& position = glm::vec3(0.0f), float yaw = 0.0f, float pitch = 0.0f, float zoom = 1.0f)
        : m_position(position)
        , m_yaw(yaw)
        , m_pitch(pitch)
        , m_zoom(zoom)
    {}

public:
    glm::vec3 evaluateDirection() const {
        glm::vec3 result(
            std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
            std::sin(glm::radians(m_pitch)),
            std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))
        );
        return glm::normalize(result);
    }

    glm::vec3 evaluateRight() const {
        glm::vec3 direction = evaluateDirection();
        glm::vec3 result = glm::cross(direction, rf::CameraConst::Up);
        return glm::normalize(result);
    }

public:
    const glm::vec3& position() const {
        return m_position;
    }

    glm::vec3& position() {
        return m_position;
    }

    float yaw() const {
        return m_yaw;
    }

    float& yaw() {
        return m_yaw;
    }

    float pitch() const {
        return m_pitch;
    }

    float& pitch() {
        return m_pitch;
    }

    float zoom() const {
        return m_zoom;
    }

    float& zoom() {
        return m_zoom;
    }
};

} // namespace rf
