#include "renderer.hpp"

#include "rf/core/engine.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/transform.hpp"
#include "rf/world/entity.hpp"

namespace rf {

void Renderer::render() {
    if (m_shader) {
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene& scene = Engine::Scene();
        m_shader->capture(*scene.get<Camera>().get());
        scene.each([this](const DrawComponent& draw) {
            m_shader->transform(draw.transform);
            m_shader->draw(draw.mesh);
        });
    }
}

} // namespace rf
