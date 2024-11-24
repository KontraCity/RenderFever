#include "rf/graphics/material.hpp"

namespace rf {
     
Graphics::Material::Material(const Texture::Pointer& diffuse, const Texture::Pointer& specular, float shininess)
    : m_diffuse(diffuse)
    , m_specular(specular)
    , m_shininess(shininess)
{}

void Graphics::Material::apply(Shader& shader) const
{
    if (m_diffuse && *m_diffuse)
        shader.set("Material.diffuse", *m_diffuse, 0);
    if (m_specular && *m_specular)
        shader.set("Material.specular", *m_specular, 1);
    shader.set("Material.shininess", m_shininess);
}

} // namespace rf
