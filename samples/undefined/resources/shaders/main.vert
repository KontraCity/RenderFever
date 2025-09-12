#version 330 core

struct FragProperties {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct LightingProperties {
    float ambient;
    float diffuse;
    float specular;
};

struct LightingAttenuation {
    float constant;
    float linear;
    float quadratic;
};

struct LightingCutoff {
    float inner;
    float outer;
};

struct PointLight {
    vec3 position;
    vec3 color;
    LightingProperties properties;
    LightingAttenuation attenuation;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    LightingProperties properties;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    LightingProperties properties;
    LightingAttenuation attenuation;
    LightingCutoff cutoff;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out FragProperties ioFragProperties;
out DirectionalLight ioDirectionalLight;
out PointLight ioPointLight;
out SpotLight ioSpotLight;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);

    // TODO: Investigate normal calculation inefficiency problem
    ioFragProperties.position = vec3(uView * uModel * vec4(aPos, 1.0f));
    ioFragProperties.normal = mat3(transpose(inverse(uView * uModel))) * aNormal; 
    ioFragProperties.texCoords = aTexCoords;

    ioDirectionalLight = uDirectionalLight;
    ioDirectionalLight.direction = vec3(uView * vec4(ioDirectionalLight.direction, 0.0f));

    ioPointLight = uPointLight;
    ioPointLight.position = vec3(uView * vec4(ioPointLight.position, 1.0f));

    ioSpotLight = uSpotLight;
    ioSpotLight.position = vec3(uView * vec4(ioSpotLight.position, 1.0f));
    ioSpotLight.direction = vec3(uView * vec4(ioSpotLight.direction, 0.0f));
}
