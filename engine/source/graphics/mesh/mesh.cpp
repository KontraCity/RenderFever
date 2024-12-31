#include "rf/graphics/mesh/mesh.hpp"

// Graphics libraries
#include <GL/glew.h>

namespace rf {

Graphics::Mesh::Mesh::Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices)
    : m_indicesCount(indices.size())
{
    // Vertex array
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    // Vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Mesh::Vertice) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Mesh::Vertice), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Mesh::Vertice), reinterpret_cast<void*>(offsetof(Graphics::Mesh::Vertice, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Graphics::Mesh::Vertice), reinterpret_cast<void*>(offsetof(Graphics::Mesh::Vertice, texCoords)));
    glEnableVertexAttribArray(2);

    // Element array
    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Graphics::Mesh::Indice) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Bind cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Graphics::Mesh::Mesh::Mesh(Mesh&& other) noexcept
    : m_vertexArray(other.m_vertexArray)
    , m_vertexBuffer(other.m_vertexBuffer)
    , m_elementBuffer(other.m_elementBuffer)
    , m_indicesCount(other.m_indicesCount)
    , m_material(other.m_material)
{
    other.m_vertexArray = 0;
    other.m_vertexBuffer = 0;
    other.m_elementBuffer = 0;
}

Graphics::Mesh::Mesh::~Mesh()
{
    if (m_elementBuffer)
        glDeleteBuffers(1, &m_elementBuffer);
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
    if (m_vertexArray)
        glDeleteVertexArrays(1, &m_vertexArray);
}

void Graphics::Mesh::Mesh::draw(Shader& shader) const
{
    // Apply
    m_material.apply(shader);

    // Draw
    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rf
