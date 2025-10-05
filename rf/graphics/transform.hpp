#pragma once

#include <rf/auxiliary/glm.hpp>

namespace rf {

namespace Graphics {
    struct Transform {
        glm::vec3 position { 0.0f };
        glm::vec3 rotation { 0.0f };
        glm::vec3 scale    { 1.0f };
    };
}

} // namespace rf
