#pragma once

// Graphics libraries
#include <GL/glew.h>

// Custom modules
#include "rf/graphics/shader.hpp"
#include "rf/graphics/cubemap.hpp"

namespace rf {

namespace Graphics
{
    class Skybox
    {
    private:
        /* OpenGL objects */
        unsigned int m_vertexArrayObject = 0;
        unsigned int m_vertexBufferObject = 0;
        unsigned int m_elementBufferObject = 0;

        /* Variables */
        Cubemap::Pointer m_cubemap;

    public:
        Skybox();

        Skybox(const Skybox& other) = delete;

        Skybox(Skybox&& other) noexcept;

        ~Skybox();

    public:
        /// @brief Draw skybox to the screen
        /// @param shader The shader to draw with
        void draw(Shader& shader) const;

    public:
        /// @brief Get skybox cubemap
        /// @return Skybox cubemap
        inline const Cubemap::Pointer& cubemap() const
        {
            return m_cubemap;
        }

        /// @brief Get skybox cubemap
        /// @return Skybox cubemap
        inline Cubemap::Pointer& cubemap()
        {
            return m_cubemap;
        }
    };
}

} // namespace rf
