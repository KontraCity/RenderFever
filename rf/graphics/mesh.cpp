#include "mesh.hpp"

#include <utility>

namespace rf {

Graphics::Mesh::Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices)
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

    glm::vec3 minBounds(FLT_MAX), maxBounds(-FLT_MAX);
    for (const auto& vertex : vertices) {
        minBounds = glm::min(minBounds, vertex.position);
        maxBounds = glm::max(maxBounds, vertex.position);
    }
    m_center = (minBounds + maxBounds) * 0.5f;
    m_radius = glm::length((maxBounds - minBounds) * 0.5f);
}

Graphics::Mesh::Mesh(Mesh&& other) noexcept
    : m_vertexArray(std::exchange(other.m_vertexArray, 0))
    , m_vertexBuffer(std::exchange(other.m_vertexBuffer, 0))
    , m_elementBuffer(std::exchange(other.m_elementBuffer, 0))
    , m_indicesCount(std::exchange(other.m_indicesCount, 0))
    , m_center(std::exchange(other.m_center, glm::vec3(0.0f)))
    , m_radius(std::exchange(other.m_radius, 0.0f))
{}

Graphics::Mesh::~Mesh() {
    free();
}

Graphics::Mesh& Graphics::Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        free();
        m_vertexArray = std::exchange(other.m_vertexArray, 0);
        m_vertexBuffer = std::exchange(other.m_vertexBuffer, 0);
        m_elementBuffer = std::exchange(other.m_elementBuffer, 0);
        m_indicesCount = std::exchange(other.m_indicesCount, 0);
        m_center = std::exchange(other.m_center, glm::vec3(0.0f));
        m_radius = std::exchange(other.m_radius, 0.0f);
    }
    return *this;
}

void Graphics::Mesh::free() {
    if (m_elementBuffer) {
        glDeleteBuffers(1, &m_elementBuffer);
        m_elementBuffer = 0;
    }

    if (m_vertexBuffer) {
        glDeleteBuffers(1, &m_vertexBuffer);
        m_vertexBuffer = 0;
    }

    if (m_vertexArray) {
        glDeleteVertexArrays(1, &m_vertexArray);
        m_vertexArray = 0;
    }

    m_indicesCount = 0;
    m_center = glm::vec3(0.0f);
    m_radius = 0.0f;
}

} // namespace rf
