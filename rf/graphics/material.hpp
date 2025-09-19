#pragma once

#include "rf/core/assets.hpp"

namespace rf {

struct Material {
    Assets::Texture texture;
    Assets::Texture specular;
    float shininess = 32.0f;
};

} // namespace rf
