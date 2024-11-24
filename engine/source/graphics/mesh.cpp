#include "rf/graphics/mesh.hpp"

namespace rf {

void Graphics::Mesh::CreateMesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, unsigned int& vao, unsigned int& vbo, unsigned int& ebo)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(offsetof(Vertice, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(offsetof(Vertice, texCoords)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indice) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

Graphics::Mesh::Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, const std::vector<Texture::Pointer>& textures)
{
    create(vertices, indices, textures);
}

Graphics::Mesh::Mesh(Mesh&& other) noexcept
    : m_vertexArrayObject(other.m_vertexArrayObject)
    , m_vertexBufferObject(other.m_vertexBufferObject)
    , m_elementBufferObject(other.m_elementBufferObject)
    , m_indicesCount(other.m_indicesCount)
    , m_textures(other.m_textures)
{
    other.m_vertexArrayObject = 0;
    other.m_vertexBufferObject = 0;
    other.m_elementBufferObject = 0;
    other.m_indicesCount = 0;
    other.m_textures.clear();
}

Graphics::Mesh::~Mesh()
{
    free();
}

void Graphics::Mesh::free()
{
    if (m_elementBufferObject)
    {
        glDeleteBuffers(1, &m_elementBufferObject);
        m_elementBufferObject = 0;
    }

    if (m_vertexBufferObject)
    {
        glDeleteBuffers(1, &m_vertexBufferObject);
        m_vertexBufferObject = 0;
    }

    if (m_vertexArrayObject)
    {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        m_vertexArrayObject = 0;
    }

    m_indicesCount = 0;
    m_textures.clear();
}

void Graphics::Mesh::create(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, const std::vector<Texture::Pointer>& textures)
{
    free(); // avoid memory leaks if load() was called already
    CreateMesh(vertices, indices, m_vertexArrayObject, m_vertexBufferObject, m_elementBufferObject);
    m_indicesCount = indices.size();
    m_textures = textures;
}

void Graphics::Mesh::draw(Shader& shader) const
{
    for (size_t index = 0, size = m_textures.size(); index < size; ++index)
    {
        std::string uniformName;
        switch (m_textures[index]->type())
        {
            case Texture::Type::Diffuse:
                uniformName = "Material.diffuse";
                break;
            case Texture::Type::Specular:
                uniformName = "Material.specular";
                break;
            default:
                // Unknown texture! Can't set anything...
                continue;
        }

        glActiveTexture(GL_TEXTURE0 + index);
        shader.set(uniformName, static_cast<int>(index));
        m_textures[index]->bind();
    }

    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rf
