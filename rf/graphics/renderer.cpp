#include "renderer.hpp"

#include "rf/core/engine.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

void Renderer::render() {
    if (m_camera && m_shader) {
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader->capture(*m_camera);
        Engine::Scene().world().each([this](const rf::Transform& transform, const rf::Mesh& mesh) {
            m_shader->transform(transform);
            m_shader->draw(mesh);
        });
    }
}

} // namespace rf
