#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 ioNormal[];

uniform mat4 uProjection;

void GenerateLine(int index) {
    gl_Position = uProjection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = uProjection * (gl_in[index].gl_Position + vec4(ioNormal[index], 0.0) * 0.5f);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
