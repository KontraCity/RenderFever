#version 330 core

struct FragProperties {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out FragProperties ioFragProperties;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);

    // TODO: Investigate normal calculation inefficiency problem
    ioFragProperties.position = vec3(uView * uModel * vec4(aPos, 1.0f));
    ioFragProperties.normal = mat3(transpose(inverse(uView * uModel))) * aNormal; 
    ioFragProperties.texCoords = aTexCoords;
}
