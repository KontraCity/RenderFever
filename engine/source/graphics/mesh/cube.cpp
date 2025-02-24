#include "rf/graphics/mesh/cube.hpp"

#include <array>

namespace rf {

namespace Model {
    constexpr std::array<Graphics::Mesh::Vertice, 24> Vertices = {{
        // Front
        { {-0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.0f, 1.0f} },
        { {+0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.0f, 0.0f} },
        { {+0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {1.0f, 0.0f} },

        // Right
        { {+0.5f, +0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}, {0.0f, 1.0f} },
        { {+0.5f, +0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}, {1.0f, 1.0f} },
        { {+0.5f, -0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}, {0.0f, 0.0f} },
        { {+0.5f, -0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}, {1.0f, 0.0f} },

        // Back
        { {+0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {0.0f, 1.0f} },
        { {-0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {1.0f, 1.0f} },
        { {+0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {0.0f, 0.0f} },
        { {-0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {1.0f, 0.0f} },

        // Left
        { {-0.5f, +0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}, {0.0f, 1.0f} },
        { {-0.5f, +0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}, {0.0f, 0.0f} },
        { {-0.5f, -0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}, {1.0f, 0.0f} },

        // Top
        { {-0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {0.0f, 1.0f} },
        { {+0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {1.0f, 1.0f} },
        { {-0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {0.0f, 0.0f} },
        { {+0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {1.0f, 0.0f} },

        // Bottom
        { {-0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}, {0.0f, 1.0f} },
        { {+0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}, {1.0f, 1.0f} },
        { {-0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}, {0.0f, 0.0f} },
        { {+0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}, {1.0f, 0.0f} },
    }};

    constexpr std::array<Graphics::Mesh::Indice, 36> Indices = {
        // Front
        0, 2, 1,
        1, 2, 3,

        // Right
        4, 6, 5,
        5, 6, 7,

        // Back
        8, 10, 9,
        9, 10, 11,

        // Left
        12, 14, 13,
        13, 14, 15,

        // Top
        16, 18, 17,
        17, 18, 19,

        // Bottom
        20, 22, 21,
        21, 22, 23,
    };
}

Graphics::Mesh::Cube::Cube()
    : Mesh({ Model::Vertices.begin(), Model::Vertices.end() }, { Model::Indices.begin(), Model::Indices.end() })
{}

Graphics::Mesh::Cube::Cube(Cube&& other) noexcept
    : Mesh(std::move(other))
    , m_transform(other.m_transform)
{}

void Graphics::Mesh::Cube::draw(Shader& shader) const {
    m_transform.apply(shader);
    Mesh::draw(shader);
}

} // namespace rf
