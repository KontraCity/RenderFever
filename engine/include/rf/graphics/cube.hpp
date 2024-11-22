#pragma once

// Graphics libraries
#include <GL/glew.h>

// Custom libraries
#include "rf/graphics/shader.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics
{
    class Cube
    {
    private:
        /* OpenGL objects */
        unsigned int m_vertexArrayObject = 0;
        unsigned int m_vertexBufferObject = 0;
        unsigned int m_elementBufferObject = 0;

        /* Variables */
        Transform m_transform;
        Material m_material;

    public:
        Cube() noexcept;

        Cube(const Cube& other) = delete;

        Cube(Cube&& other) noexcept;

        ~Cube();

    public:
        /// @brief Draw cube to the screen
        /// @param shader The shader to draw with
        void draw(Shader& shader) const;

    public:
        /// @brief Get cube transform
        /// @return Cube transform
        inline const Transform& transform() const
        {
            return m_transform;
        }

        /// @brief Get cube transform
        /// @return Cube transform
        inline Transform& transform()
        {
            return m_transform;
        }

        /// @brief Get cube material
        /// @return Cube material
        inline const Material& material() const
        {
            return m_material;
        }

        /// @brief Get cube material
        /// @return Cube material
        inline Material& material()
        {
            return m_material;
        }
    };
}

} // namespace rf
