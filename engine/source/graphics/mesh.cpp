#include "rf/graphics/mesh.hpp"

namespace rf {

Mesh::Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices)
    : m_indicesCount(indices.size()) {
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(offsetof(Vertice, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(offsetof(Vertice, texCoords)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indice) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vertexArray(other.m_vertexArray)
    , m_vertexBuffer(other.m_vertexBuffer)
    , m_elementBuffer(other.m_elementBuffer)
    , m_indicesCount(other.m_indicesCount) {
    other.reset();
}

Mesh::~Mesh() {
    if (m_elementBuffer)
        glDeleteBuffers(1, &m_elementBuffer);
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
    if (m_vertexArray)
        glDeleteVertexArrays(1, &m_vertexArray);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    m_vertexArray = other.m_vertexArray;
    m_vertexBuffer = other.m_vertexBuffer;
    m_elementBuffer = other.m_elementBuffer;
    m_indicesCount = other.m_indicesCount;
    other.reset();
    return *this;
}

void Mesh::reset() {
    m_vertexArray = 0;
    m_vertexBuffer = 0;
    m_elementBuffer = 0;
    m_indicesCount = 0;
}

} // namespace rf
