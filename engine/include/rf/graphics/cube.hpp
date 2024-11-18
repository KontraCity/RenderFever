#pragma once

// Graphics libraries
#include <GL/glew.h>

// Custom libraries
#include "rf/graphics/shader.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics
{
    class Cube : public Transform
    {
    private:
        unsigned int m_vertexArrayObject = 0;
        unsigned int m_vertexBufferObject = 0;
        unsigned int m_elementBufferObject = 0;

    public:
        Cube();

        Cube(const Cube& other) = delete;

        Cube(Cube&& other) noexcept;

        ~Cube();

        /// @brief Draw cube to the screen
        /// @param shader The shader to draw with
        void draw(Shader& shader) const;
    };
}

} // namespace rf
