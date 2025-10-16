#pragma once

#include <rf/auxiliary/glm.hpp>

#include <rf/graphics/rotation.hpp>

namespace rf {

namespace Graphics {
    struct Transform {
        glm::vec3 position { 0.0f };
        Rotation rotation = {};
        glm::vec3 scale { 1.0f };
    };
}

} // namespace rf
