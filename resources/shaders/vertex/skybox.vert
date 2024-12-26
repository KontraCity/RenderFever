#version 330 core

// Layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoords;

// Output
out vec3 ioTexCoords;

// Uniforms
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = (uProjection * uView * vec4(aPos, 1.0)).xyww;
    ioTexCoords = aTexCoords;
}
