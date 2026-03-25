#pragma once

#include <rf/auxiliary/glm.hpp>

#include <rf/graphics/camera.hpp>
#include <rf/graphics/dimensions.hpp>
#include <rf/graphics/rotation.hpp>
#include <rf/graphics/transform.hpp>

namespace rf {
    
namespace Math {
    glm::quat EvaluateOrientation(const Graphics::Rotation& rotation);

    glm::vec3 EvaluateUp(const Graphics::Camera& camera);

    glm::vec3 EvaluateRight(const Graphics::Camera& camera);

    glm::vec3 EvaluateDirection(const Graphics::Camera& camera);

    glm::mat4 EvaluateView(const Graphics::Camera& camera);

    glm::mat4 EvaluateProjection(const Graphics::Camera& camera, const Graphics::Dimensions& dimensions);

    glm::mat4 EvaluateModel(const Graphics::Transform& transform);

    void DirectCameraAtSphere(Graphics::Camera& camera, glm::vec3 center, float radius);
}

} // namespace rf
