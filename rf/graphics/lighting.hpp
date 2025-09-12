#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace rf {

namespace Lighting {
    struct Color {
        uint8_t red = 255;
        uint8_t green = 255;
        uint8_t blue = 255;
    };

    struct Properties {
        float ambient = 0.2f;
        float diffuse = 0.5f;
        float specular = 1.0f;
    };

    struct Attenuation {
        float constant = 0.2f;
        float linear = 0.09f;
        float quadratic = 0.032f;
    };

    struct Cutoff {
        float inner = 10.0f;
        float outer = 12.0f;
    };
}

struct PointLight {
    glm::vec3 position = {};
    Lighting::Color color = {};
    Lighting::Properties properties = {};
    Lighting::Attenuation attenuation = {};
};

struct DirectionalLight {
    glm::vec3 direction = {};
    Lighting::Color color = {};
    Lighting::Properties properties = {};
};

struct SpotLight {
    glm::vec3 position = {};
    glm::vec3 direction = {};
    Lighting::Color color = {};
    Lighting::Properties properties = {};
    Lighting::Attenuation attenuation = {};
    Lighting::Cutoff cutoff = {};
};

} // namespace rf
