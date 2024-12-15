#version 330 core

// Input
in vec3 ioTexCoords;

// Output
out vec4 ioFragColor;

// Uniforms
uniform samplerCube uCubemap;

void main()
{
    ioFragColor = texture(uCubemap, ioTexCoords);
}
