#include "rf/graphics/lighting/spot_light.hpp"

namespace rf {

void Graphics::Lighting::SpotLight::illuminate(Shader& shader) const
{
    shader.set("SpotLight.properties", m_properties);
    shader.set("SpotLight.attenuation", m_attenuation);
    shader.set("SpotLight.cutoff", m_cutoff);
    shader.set("SpotLight.color", m_color);
    shader.set("SpotLight.position", m_position);
    shader.set("SpotLight.direction", m_direction);
}

} // namespace rf
