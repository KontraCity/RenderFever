#pragma once

#include "rf/graphics/texture.hpp"

namespace rf {

struct Material {
    Texture::Pointer texture;
    Texture::Pointer specular;
    float shininess = 32.0f;
};

} // namespace rf
