#pragma once

// STL modules
#include <vector>

// Graphics libraries
#include <GL/glew.h>
#include <glm/glm.hpp>

// Custom modules
#include "rf/graphics/shader.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics
{
    class Mesh
    {
    public:
        struct Vertice
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        using Indice = unsigned int;

    private:
        /// @brief Create mesh
        /// @param vertices Mesh vertices
        /// @param indices Mesh indices
        /// @param vao Result vertex array object
        /// @param vbo Result vertex buffer object
        /// @param ebo Result element buffer object
        static void CreateMesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, unsigned int& vao, unsigned int& vbo, unsigned int& ebo);

    private:
        /* OpenGL objects */
        unsigned int m_vertexArrayObject = 0;
        unsigned int m_vertexBufferObject = 0;
        unsigned int m_elementBufferObject = 0;

        /* Variables */
        size_t m_indicesCount = 0;
        std::vector<Texture::Pointer> m_textures;

    public:
        /// @brief Create mesh
        /// @param vertices Mesh vertices
        /// @param indices Mesh indices
        /// @param textures Mesh textures
        Mesh(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, const std::vector<Texture::Pointer>& textures);

        Mesh() = default;

        Mesh(const Mesh& other) = delete;

        Mesh(Mesh&& other) noexcept;

        ~Mesh();

    private:
        /// @brief Free allocated resources
        void free();

    public:
        /// @brief Create mesh
        /// @param vertices Mesh vertices
        /// @param indices Mesh indices
        /// @param textures Mesh textures
        void create(const std::vector<Vertice>& vertices, const std::vector<Indice>& indices, const std::vector<Texture::Pointer>& textures);

        /// @brief Draw mesh to the screen
        /// @param shader The shader to draw with
        void draw(Shader& shader) const;
    };
}

} // namespace rf
