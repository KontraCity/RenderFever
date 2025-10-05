#include "math.hpp"

namespace rf {

glm::quat Math::EvaluateOrientation(const Graphics::Camera& camera) {
    glm::quat pitchRotation = glm::angleAxis(glm::radians(camera.pitch), Graphics::Camera::Right);
    glm::quat yawRotation = glm::angleAxis(glm::radians(-camera.yaw), Graphics::Camera::Up);
    return glm::normalize(yawRotation * pitchRotation);
}

glm::vec3 Math::EvaluateUp(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera);
    return glm::normalize(orientation * Graphics::Camera::Up);
}

glm::vec3 Math::EvaluateRight(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera);
    return glm::normalize(orientation * Graphics::Camera::Right);
}

glm::vec3 Math::EvaluateDirection(const Graphics::Camera& camera) {
    glm::quat orientation = EvaluateOrientation(camera);
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
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(transform.rotation)));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
    return translation * rotation * scale;
}

} // namespace rf
