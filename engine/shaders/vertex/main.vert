#version 330 core

// Vertice layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Output
out vec3 ioFragPos;
out vec3 ioNormal;
out vec2 ioTexCoords;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    // TODO: Investigate normal calculation inefficiency problem
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
    ioFragPos = vec3(uView * uModel * vec4(aPos, 1.0f));
    ioNormal = mat3(transpose(inverse(uView * uModel))) * aNormal;
    ioTexCoords = aTexCoords;
}
