#pragma once

#include <cstdint>

#include <rf/auxiliary/gl.hpp>
#include <rf/auxiliary/glm.hpp>

namespace rf {

namespace Graphics {
    static glm::vec3 VectorColor(uint8_t red, uint8_t green, uint8_t blue) {
        return {
            red   / 255.0f,
            green / 255.0f,
            blue  / 255.0f,
        };
    }

    enum class LightType : GLint {
        DirectionalLight = 0,   // Radiates in parallel rays, has a direction but not a position, like a sun
        PointLight       = 1,   // Radiates from a position in all directions, like a bulb
        SpotLight        = 2,   // Radiates from a position in one direction limited by an angle cone, like a flashlight
    };

    struct Light {
        glm::vec3 position  { 0.0 };                // Point and spot lights only
        float _padding1;
        glm::vec3 direction { 1.0f, 0.0f, 0.0f };   // Directional and spot lights only
        float _padding2;
        glm::vec3 color = VectorColor(255, 255, 255);
        LightType type  = LightType::DirectionalLight;

        // Properties
        float ambientProperty  = 0.2f;
        float diffuseProperty  = 0.5f;
        float specularProperty = 1.0f;

        // Attenuation
        float constantAttenuation  = 0.2f;      // Point and spot lights only
        float linearAttenuation    = 0.09f;     // Point and spot lights only
        float quadraticAttenuation = 0.032f;    // Point and spot lights only

        // Cutoff
        float spotInnerCutoff = 10.0f;  // Spot light only
        float spotOuterCutoff = 12.0f;  // Spot light only
    };
}

} // namespace rf
