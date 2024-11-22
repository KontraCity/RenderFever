#version 330 core

struct FragProperties
{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Material
{
    sampler2D texture;
    sampler2D specular;
    float shininess;
};

struct LightingProperties
{
    float ambient;
    float diffuse;
    float specular;
};

struct LightingAttenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct LightingCutoff
{
    float inner;
    float outer;
};

struct DirectionalLight
{
    LightingProperties properties;
    vec3 color;
    vec3 direction;
};

struct PointLight
{
    LightingProperties properties;
    LightingAttenuation attenuation;
    vec3 color;
    vec3 position;
};

struct SpotLight
{
    LightingProperties properties;
    LightingAttenuation attenuation;
    LightingCutoff cutoff;
    vec3 color;
    vec3 position;
    vec3 direction;
};

// Vertice layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Output
out FragProperties ioFragProperties;
out DirectionalLight ioDirectionalLight;
out PointLight ioPointLight;
out SpotLight ioSpotLight;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);

    // Fragment properties
    ioFragProperties.position = vec3(uView * uModel * vec4(aPos, 1.0f));
    ioFragProperties.normal = mat3(transpose(inverse(uView * uModel))) * aNormal; // TODO: Investigate normal calculation inefficiency problem
    ioFragProperties.texCoords = aTexCoords;

    // Directional light
    ioDirectionalLight = uDirectionalLight;
    ioDirectionalLight.direction = vec3(uView * vec4(ioDirectionalLight.direction, 0.0f));

    // Point light
    ioPointLight = uPointLight;
    ioPointLight.position = vec3(uView * vec4(ioPointLight.position, 1.0f));

    // Spot light
    ioSpotLight = uSpotLight;
    ioSpotLight.position = vec3(uView * vec4(ioSpotLight.position, 1.0f));
    ioSpotLight.direction = vec3(uView * vec4(ioSpotLight.direction, 0.0f));
}
