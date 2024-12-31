#pragma once

// Custom modules
#include "rf/graphics/mesh/mesh.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics
{
    namespace Mesh
    {
        class Plane : public Mesh
        {
        private:
            Transform m_transform;

        public:
            Plane();

            Plane(Plane&& other) noexcept;

        public:
            /// @brief Draw plane to the screen
            /// @param shader The shader to draw with
            virtual void draw(Shader& shader) const;

        public:
            /// @brief Get plane transform
            /// @return Plane transform
            inline const Transform& transform() const
            {
                return m_transform;
            }

            /// @brief Get plane transform
            /// @return Plane transform
            inline Transform& transform()
            {
                return m_transform;
            }
        };
    }
}

} // namespace rf
