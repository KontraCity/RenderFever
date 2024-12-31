#pragma once

// Custom modules
#include "rf/graphics/shader.hpp"
#include "rf/graphics/cubemap.hpp"

namespace rf {

namespace Graphics
{
    class Skybox
    {
    private:
        /* OpenGL members */
        unsigned int m_vertexArray = 0;
        unsigned int m_vertexBuffer = 0;
        unsigned int m_elementBuffer = 0;

        /* Other members */
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
