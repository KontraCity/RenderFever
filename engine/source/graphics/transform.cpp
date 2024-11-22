#include "rf/graphics/transform.hpp"

namespace rf {

Graphics::Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) noexcept
    : m_position(position)
    , m_rotation(rotation)
    , m_scale(scale)
{}

void Graphics::Transform::apply(Shader& shader) const
{
    // TODO: Rotate model around a single axis rather than three
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_scale);
    shader.set("Model", model);
}

} // namespace rf
