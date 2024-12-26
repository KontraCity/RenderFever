#version 330 core

// Layout
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// Input
in vec3 ioNormal[];

// Uniforms
uniform mat4 uProjection;

void GenerateLine(int index)
{
    gl_Position = uProjection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = uProjection * (gl_in[index].gl_Position + vec4(ioNormal[index], 0.0) * 0.5f);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
