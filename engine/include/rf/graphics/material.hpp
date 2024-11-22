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
        Texture::Pointer m_texture;
        Texture::Pointer m_specular;
        float m_shininess = 32.0f;

    public:
        Material() noexcept = default;

        /// @brief Create material
        /// @param texture Material texture
        /// @param specular Material specular map
        /// @param shininess Material shininess
        Material(const Texture::Pointer& texture, const Texture::Pointer& specular, float shininess) noexcept;

    public:
        /// @brief Apply material
        /// @param shader The shader to apply material to
        void apply(Shader& shader) const;

    public:
        /// @brief Get material texture
        /// @return Material texture
        inline const Texture::Pointer& texture() const
        {
            return m_texture;
        }

        /// @brief Get material texture
        /// @return Material texture
        inline Texture::Pointer& texture()
        {
            return m_texture;
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
