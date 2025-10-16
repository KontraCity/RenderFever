#include "math.hpp"

#include <cmath>

namespace rf {

glm::quat Math::EvaluateOrientation(const Graphics::Rotation& rotation) {
    glm::quat pitchRotation = glm::angleAxis(glm::radians(rotation.pitch), Graphics::Camera::Right);
    glm::quat yawRotation   = glm::angleAxis(glm::radians(-rotation.yaw), Graphics::Camera::Up);
    glm::quat rollRotation  = glm::angleAxis(glm::radians(rotation.roll), Graphics::Camera::Direction);
    return glm::normalize(yawRotation * pitchRotation * rollRotation);
}

glm::vec3 Math::EvaluateUp(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera.rotation);
    return glm::normalize(orientation * Graphics::Camera::Up);
}

glm::vec3 Math::EvaluateRight(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera.rotation);
    return glm::normalize(orientation * Graphics::Camera::Right);
}

glm::vec3 Math::EvaluateDirection(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera.rotation);
    return glm::normalize(orientation * Graphics::Camera::Direction);
}

glm::mat4 Math::EvaluateView(const Graphics::Camera& camera) {
    glm::vec3 up = EvaluateUp(camera);
    glm::vec3 direction = EvaluateDirection(camera);
    return glm::lookAt(camera.position, camera.position + direction, up);
}

glm::mat4 Math::EvaluateProjection(const Graphics::Camera& camera, const Graphics::Dimensions& dimensions) {
    float aspectRatio = static_cast<float>(dimensions.width) / dimensions.height;
    if (camera.projectionMode == Graphics::ProjectionMode::Perspective) {
        return glm::perspective(
            glm::radians(Graphics::Camera::PerspectiveFov / camera.zoom),
            aspectRatio, Graphics::Camera::NearPlane, Graphics::Camera::FarPlane
        );
    }

    float scale = Graphics::Camera::OrthographicScale / camera.zoom;
    float halfWidth = scale / 2 * aspectRatio;
    float halfHeight = scale / 2;
    return glm::ortho(
        -halfWidth, +halfWidth,
        -halfHeight, +halfHeight,
        Graphics::Camera::NearPlane, Graphics::Camera::FarPlane
    );
}

glm::mat4 Math::EvaluateModel(const Graphics::Transform& transform) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.position);
    glm::mat4 rotation = glm::toMat4(EvaluateOrientation(transform.rotation));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
    return translation * rotation * scale;
}

void Math::DirectCameraAtMesh(Graphics::Camera& camera, const Graphics::Mesh& mesh) {
    constexpr float CameraFovRadians = glm::radians(Graphics::Camera::PerspectiveFov);

    glm::vec3 direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
    float distance = mesh.radius() / std::sin(CameraFovRadians * 0.5f);
    camera.position = mesh.center() - direction * distance;

    glm::vec3 lookDirection = glm::normalize(mesh.center() - camera.position);
    camera.rotation.pitch = glm::degrees(std::asin(lookDirection.y));
    camera.rotation.yaw = glm::degrees(std::atan2(lookDirection.x, -lookDirection.z));
    camera.rotation.roll = 0.0f;
    camera.zoom = 1.0f;
}

} // namespace rf
