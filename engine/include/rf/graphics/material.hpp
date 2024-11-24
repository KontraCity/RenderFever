#pragma once

// Custom modules
#include "rf/graphics/shader.hpp"
#include "rf/graphics/texture.hpp"

namespace rf {

namespace Graphics
{
    class Material
    {
    private:
        Texture::Pointer m_diffuse;
        Texture::Pointer m_specular;
        float m_shininess = 32.0f;

    public:
        Material() = default;

        /// @brief Create material
        /// @param diffuse Material diffuse texture
        /// @param specular Material specular map
        /// @param shininess Material shininess
        Material(const Texture::Pointer& diffuse, const Texture::Pointer& specular, float shininess);

    public:
        /// @brief Apply material
        /// @param shader The shader to apply material to
        void apply(Shader& shader) const;

    public:
        /// @brief Get material diffuse texture
        /// @return Material diffuse texture
        inline const Texture::Pointer& diffuse() const
        {
            return m_diffuse;
        }

        /// @brief Get material diffuse texture
        /// @return Material diffuse texture
        inline Texture::Pointer& diffuse()
        {
            return m_diffuse;
        }

        /// @brief Get material specular map
        /// @return Material pecular map
        inline const Texture::Pointer& specular() const
        {
            return m_specular;
        }

        /// @brief Get material specular map
        /// @return Material specular map
        inline Texture::Pointer& specular()
        {
            return m_specular;
        }

        /// @brief Get material shininess
        /// @return Material shininess
        inline float shininess() const
        {
            return m_shininess;
        }

        /// @brief Get material shininess
        /// @return Material shininess
        inline float& shininess()
        {
            return m_shininess;
        }
    };
}

} // namespace rf
