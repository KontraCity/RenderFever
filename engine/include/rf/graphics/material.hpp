#pragma once

#include "rf/graphics/shader.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics {
    class Material {
    private:
        Texture::Pointer m_diffuse;
        Texture::Pointer m_specular;
        float m_shininess = 32.0f;

    public:
        Material() = default;

        Material(const Texture::Pointer& diffuse, const Texture::Pointer& specular, float shininess);

    public:
        void apply(Shader& shader) const;

    public:
        inline const Texture::Pointer& diffuse() const {
            return m_diffuse;
        }

        inline Texture::Pointer& diffuse() {
            return m_diffuse;
        }

        inline const Texture::Pointer& specular() const {
            return m_specular;
        }

        inline Texture::Pointer& specular() {
            return m_specular;
        }

        inline float shininess() const {
            return m_shininess;
        }

        inline float& shininess() {
            return m_shininess;
        }
    };
}

} // namespace rf
