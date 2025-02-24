#include "rf/graphics/mesh/plane.hpp"

#include <array>

namespace rf {

namespace Model {
    constexpr std::array<Graphics::Mesh::Vertice, 4> Vertices = {{
        { {-0.5f, +0.0f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {0.0f, 1.0f} },
        { {+0.5f, +0.0f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {1.0f, 1.0f} },
        { {-0.5f, +0.0f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {0.0f, 0.0f} },
        { {+0.5f, +0.0f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {1.0f, 0.0f} },
    }};

    constexpr std::array<Graphics::Mesh::Indice, 6> Indices = {
        0,  2,  1,
        1,  2,  3,
    };
}

Graphics::Mesh::Plane::Plane()
    : Mesh({ Model::Vertices.begin(), Model::Vertices.end() }, { Model::Indices.begin(), Model::Indices.end() })
{}

Graphics::Mesh::Plane::Plane(Plane&& other) noexcept
    : Mesh(std::move(other))
    , m_transform(other.m_transform)
{}

void Graphics::Mesh::Plane::draw(Shader& shader) const {
    m_transform.apply(shader);
    Mesh::draw(shader);
}

} // namespace rf
