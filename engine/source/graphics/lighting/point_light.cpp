#include "rf/graphics/lighting/point_light.hpp"

namespace rf {

void Graphics::Lighting::PointLight::illuminate(Shader& shader) const
{
    shader.set("PointLight.properties", m_properties);
    shader.set("PointLight.attenuation", m_attenuation);
    shader.set("PointLight.color", m_color);
    shader.set("PointLight.position", m_position);
}

} // namespace rf
