#pragma once

#include <cstdint>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace rf {

static glm::vec3 VectorColor(uint8_t red, uint8_t green, uint8_t blue) {
    return {
        red   / 255.0f,
        green / 255.0f,
        blue  / 255.0f,
    };
}

enum class LightType : GLint {
    DirectionalLight = 0,
    PointLight       = 1,
    SpotLight        = 2,
};

struct Light {
    glm::vec3 position  = { 0.0f, 0.0f, 0.0f };     // Point and spot lights only
    float _padding1;                                // [Padding, unused]
    glm::vec3 direction = { 1.0f, 0.0f, 0.0f };     // Directional and spot lights only
    float _padding2;                                // [Padding, unused]
    glm::vec3 color     = { 1.0f, 1.0f, 1.0f };
    LightType type = LightType::DirectionalLight;

    // Properties
    float ambientProperty  = 0.2f;
    float diffuseProperty  = 0.5f;
    float specularProperty = 1.0f;

    // Attenuation
    float constantAttenuation  = 0.2f;              // Point and spot lights only
    float linearAttenuation    = 0.09f;             // Point and spot lights only
    float quadraticAttenuation = 0.032f;            // Point and spot lights only

    // Cutoff
    float spotInnerCutoff = 10.0f;                  // Spot light only
    float spotOuterCutoff = 12.0f;                  // Spot light only
};

} // namespace rf
