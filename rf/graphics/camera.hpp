#pragma once

#include <rf/auxiliary/glm.hpp>

#include <rf/graphics/rotation.hpp>

namespace rf {

namespace Graphics {
    enum class ProjectionMode {
        Perspective,    // Objects shrink with distance, mimicking real life vision
        Orthographic,   // Objects keep the same size regardless of distance
    };

    constexpr const char* ProjectionModeToString(ProjectionMode mode) {
        switch (mode) {
            case ProjectionMode::Perspective:   return "Perspective";
            case ProjectionMode::Orthographic:  return "Orthographic";
            default:                            return "Unknown";
        }
    }

    struct Camera {
        static constexpr glm::vec3 Up           { 0.0f, 1.0f, 0.0f };
        static constexpr glm::vec3 Right        { 1.0f, 0.0f, 0.0f };
        static constexpr glm::vec3 Direction    { 0.0f, 0.0f, -1.0f };
        static constexpr float PerspectiveFov    = 45.0f;   // FOV at 1x zoom
        static constexpr float OrthographicScale = 5.0f;    // Scale at 1x zoom
        static constexpr float NearPlane         = 0.1f;
        static constexpr float FarPlane          = 10000.0f;

        ProjectionMode projectionMode = ProjectionMode::Perspective;
        glm::vec3 position { 0.0f };
        Rotation rotation = {};
        float zoom = 1.0f;
    };
}

} // namespace rf
