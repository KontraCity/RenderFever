#version 330 core

in vec3 ioTexCoords;

out vec4 ioFragColor;

uniform samplerCube uCubemap;

void main() {
    ioFragColor = texture(uCubemap, ioTexCoords);
}
