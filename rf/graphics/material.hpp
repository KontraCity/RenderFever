#pragma once

#include <rf/resources/library.hpp>

namespace rf {

namespace Graphics {
    struct Material {
        Resources::Shader shader;
        Resources::Texture diffuse;
        Resources::Texture specular;
        float shininess = 32.0f;
    };
}

} // namespace rf
