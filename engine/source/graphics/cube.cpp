#include "rf/graphics/cube.hpp"

namespace Model
{
    constexpr float Vertices[] = {
        // Front
        // Coordinates         Normal coordinates    Texture coordinates
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,

        // Right
        // Coordinates         Normal coordinates    Texture coordinates
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        // Back
        // Coordinates         Normal coordinates    Texture coordinates
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,

        // Left
        // Coordinates         Normal coordinates    Texture coordinates
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        // Top
        // Coordinates         Normal coordinates    Texture coordinates
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,

        // Bottom
        // Coordinates         Normal coordinates    Texture coordinates
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    };

    constexpr int Indices[] = {
        // Front
        0, 1, 2,
        1, 2, 3,

        // Right
        4, 5, 6,
        5, 6, 7,

        // Back
        8, 9, 10,
        9, 10, 11,

        // Left
        12, 13, 14,
        13, 14, 15,

        // Top
        16, 17, 18,
        17, 18, 19,

        // Bottom
        20, 21, 22,
        21, 22, 23,
    };
}

namespace rf {

Graphics::Cube::Cube() noexcept
{
    // Vertex array object
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Vertex buffer object
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertices), Model::Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // Element buffer object
    glGenBuffers(1, &m_elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Model::Indices), Model::Indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

Graphics::Cube::Cube(Cube&& other) noexcept
    : m_vertexArrayObject(other.m_vertexArrayObject)
    , m_vertexBufferObject(other.m_vertexBufferObject)
    , m_elementBufferObject(other.m_elementBufferObject)
{
    other.m_vertexArrayObject = 0;
    other.m_vertexBufferObject = 0;
    other.m_elementBufferObject = 0;
}

Graphics::Cube::~Cube()
{
    if (m_elementBufferObject)
        glDeleteBuffers(1, &m_elementBufferObject);
    if (m_vertexBufferObject)
        glDeleteBuffers(1, &m_vertexBufferObject);
    if (m_vertexArrayObject)
        glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Graphics::Cube::draw(Shader& shader) const
{
    // Apply
    m_transform.apply(shader);
    m_material.apply(shader);

    // Draw
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, sizeof(Model::Indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rf
