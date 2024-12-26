#version 330 core

// Layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Output
out vec3 ioNormal;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;

void main()
{
    gl_Position = uView * uModel * vec4(aPos, 1.0f); 
    ioNormal = normalize(mat3(transpose(inverse(uView * uModel))) * aNormal);
}
