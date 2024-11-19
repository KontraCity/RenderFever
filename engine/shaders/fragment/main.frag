#version 330 core

// Input
in vec3 ioFragPos;
in vec3 ioNormal;
in vec2 ioTexCoords;

// Output
out vec4 ioFragColor;

// Uniforms
uniform sampler2D uTexture;

void main()
{
    ioFragColor = texture(uTexture, ioTexCoords);
}
