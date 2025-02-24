#pragma once

#include "rf/graphics/mesh/mesh.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

namespace Graphics {
    namespace Mesh {
        class Cube : public Mesh {
        private:
            Transform m_transform;

        public:
            Cube();

            Cube(Cube&& other) noexcept;

        public:
            virtual void draw(Shader& shader) const;

        public:
            inline const Transform& transform() const {
                return m_transform;
            }

            inline Transform& transform() {
                return m_transform;
            }
        };
    }
}

} // namespace rf
