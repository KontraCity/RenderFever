#pragma once

// Custom modules
#include "rf/graphics/mesh/mesh.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics
{
    namespace Mesh
    {
        class Cube : public Mesh
        {
        private:
            Transform m_transform;

        public:
            Cube();

            Cube(Cube&& other) noexcept;

        public:
            /// @brief Draw cube to the screen
            /// @param shader The shader to draw with
            virtual void draw(Shader& shader) const;

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
        };
    }
}

} // namespace rf
