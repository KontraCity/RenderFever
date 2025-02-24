#pragma once

#include "rf/graphics/shader.hpp"
#include "rf/graphics/cubemap.hpp"

namespace rf {

namespace Graphics {
    class Skybox {
    private:
        unsigned int m_vertexArray = 0;
        unsigned int m_vertexBuffer = 0;
        unsigned int m_elementBuffer = 0;

        Cubemap::Pointer m_cubemap;

    public:
        Skybox();

        Skybox(const Skybox& other) = delete;

        Skybox(Skybox&& other) noexcept;

        ~Skybox();

    public:
        void draw(Shader& shader) const;

    public:
        inline const Cubemap::Pointer& cubemap() const {
            return m_cubemap;
        }

        inline Cubemap::Pointer& cubemap() {
            return m_cubemap;
        }
    };
}

} // namespace rf
