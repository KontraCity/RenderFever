#include "../include/version.glsl"
#include "../include/essentials.glsl"
#include "../include/lighting.glsl"
#include "../include/material.glsl"

in FragProperties ioFragProperties;
uniform Material uMaterial;
out vec4 ioFragColor;

layout(std430, binding = 0) buffer LightStorage {
    int lsLightCount;
    Light lsLights[];
};

void main() {
    vec3 result = vec3(0.0f);
    for (int index = 0; index < lsLightCount; ++index) {
        switch (lsLights[index].type) {
            case DirectionalLightType:
                result += CalcDirectionalLight(ioFragProperties, lsLights[index], uMaterial);
                break;
            case PointLightType:
                result += CalcPointLight(ioFragProperties, lsLights[index], uMaterial);
                break;
            case SpotLightType:
                result += CalcSpotLight(ioFragProperties, lsLights[index], uMaterial);
                break;
        }
    }
    ioFragColor = vec4(result, 1.0f);
}
