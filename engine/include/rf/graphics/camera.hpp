#pragma once

#include <cstdint>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace rf {

namespace CameraConst {
    constexpr glm::vec3 Up(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 Right(1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 Direction(0.0f, 0.0f, -1.0f);

    constexpr float PerspectiveFov = 45.0f;
    constexpr float OrthographicScale = 5.0f;

    constexpr float NearPlane = 0.1f;
    constexpr float FarPlane = 10000.0f;
}

enum class ProjectionMode : uint8_t {
    Perspective,
    Orthographic,
};

class Camera {
protected:
    ProjectionMode m_projectionMode = ProjectionMode::Perspective;
    glm::vec3 m_position = glm::vec3(0.0f);
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_zoom = 1.0f;

public:
    Camera() = default;

    Camera(ProjectionMode projectionMode, const glm::vec3& position, float yaw, float pitch, float zoom)
        : m_projectionMode(projectionMode)
        , m_position(position)
        , m_yaw(yaw)
        , m_pitch(pitch)
        , m_zoom(zoom)
    {}

private:
    glm::quat evaluateOrientation() const {
        glm::quat pitchRotation = glm::angleAxis(glm::radians(m_pitch), CameraConst::Right);
        glm::quat yawRotation = glm::angleAxis(glm::radians(-m_yaw), CameraConst::Up);
        return glm::normalize(yawRotation * pitchRotation);
    }

public:
    glm::vec3 evaluateUp() const {
        glm::quat orientation = evaluateOrientation();
        return glm::normalize(orientation * CameraConst::Up);
    }

    glm::vec3 evaluateRight() const {
        glm::quat orientation = evaluateOrientation();
        return glm::normalize(orientation * CameraConst::Right);
    }

    glm::vec3 evaluateDirection() const {
        glm::quat orientation = evaluateOrientation();
        return glm::normalize(orientation * CameraConst::Direction);
    }

    glm::mat4 evaluateView() const {
        glm::vec3 direction = evaluateDirection();
        return glm::lookAt(m_position, m_position + direction, CameraConst::Up);
    }

    glm::mat4 evaluateProjection(float aspectRatio) const {
        if (m_projectionMode == ProjectionMode::Perspective) {
            return glm::perspective(
                glm::radians(CameraConst::PerspectiveFov / m_zoom),
                aspectRatio,
                CameraConst::NearPlane,
                CameraConst::FarPlane
            );
        }

        float scale = CameraConst::OrthographicScale / m_zoom;
        float halfWidth = scale / 2 * aspectRatio;
        float halfHeight = scale / 2;
        return glm::ortho(
            -halfWidth, +halfWidth,
            -halfHeight, +halfHeight,
            CameraConst::NearPlane,
            CameraConst::FarPlane
        );
    }

public:
    ProjectionMode projectionMode() const {
        return m_projectionMode;
    }

    ProjectionMode& projectionMode() {
        return m_projectionMode;
    }

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
