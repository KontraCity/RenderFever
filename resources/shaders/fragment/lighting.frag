#version 330 core

out vec4 ioFragColor;

uniform vec3 uLightColor;

void main() {
    ioFragColor = vec4(uLightColor, 1.0f);
}
