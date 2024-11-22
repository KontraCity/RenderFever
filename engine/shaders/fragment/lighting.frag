#version 330 core

// Output
out vec4 ioFragColor;

// Uniforms
uniform vec3 uLightColor;

void main()
{
    ioFragColor = vec4(uLightColor, 1.0f);
}
