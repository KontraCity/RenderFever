#include "rf/graphics/material.hpp"

namespace rf {
     
Graphics::Material::Material(const Texture::Pointer& texture, const Texture::Pointer& specular, float shininess) noexcept
    : m_texture(texture)
    , m_specular(specular)
    , m_shininess(shininess)
{}

void Graphics::Material::apply(Shader& shader) const
{
    if (m_texture && *m_texture)
        shader.set("Material.texture", *m_texture, 0);
    if (m_specular && *m_specular)
        shader.set("Material.specular", *m_specular, 1);
    shader.set("Material.shininess", m_shininess);
}

} // namespace rf
