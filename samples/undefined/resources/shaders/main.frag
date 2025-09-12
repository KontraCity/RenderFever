#version 330 core

struct FragProperties {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Material {
    sampler2D texture;
    sampler2D specular;
    float shininess;
};

in FragProperties ioFragProperties;

out vec4 ioFragColor;

uniform Material uMaterial;

void main() {
    ioFragColor = vec4(texture(uMaterial.texture, ioFragProperties.texCoords).rgb, 1.0f);
}
