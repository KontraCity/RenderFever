#pragma once

#include <rf/auxiliary/glm.hpp>

#include <rf/graphics/camera.hpp>
#include <rf/graphics/dimensions.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/transform.hpp>

namespace rf {
    
namespace Math {
    glm::quat EvaluateOrientation(const Graphics::Camera& camera);

    glm::vec3 EvaluateUp(const Graphics::Camera& camera);

    glm::vec3 EvaluateRight(const Graphics::Camera& camera);

    glm::vec3 EvaluateDirection(const Graphics::Camera& camera);

    glm::mat4 EvaluateView(const Graphics::Camera& camera);

    glm::mat4 EvaluateProjection(const Graphics::Camera& camera, const Graphics::Dimensions& dimensions);

    glm::mat4 EvaluateModel(const Graphics::Transform& transform);

    void DirectCameraAtMesh(Graphics::Camera& camera, const Graphics::Mesh& mesh);
}

} // namespace rf
