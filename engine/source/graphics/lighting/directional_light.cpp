#include "rf/graphics/lighting/directional_light.hpp"

namespace rf {

void Graphics::Lighting::DirectionalLight::illuminate(Shader& shader) const
{
    shader.set("DirectionalLight.properties", m_properties);
    shader.set("DirectionalLight.color", m_color);
    shader.set("DirectionalLight.direction", m_direction);
}

} // namespace rf