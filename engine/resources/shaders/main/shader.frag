#version 330 core

// Input
in vec3 ioFragPos;
in vec3 ioNormal;
in vec2 ioTexCoords;

// Output
out vec4 ioFragColor;

void main()
{
    ioFragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
