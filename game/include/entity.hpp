#pragma once

#include <memory>

#include <rf/graphics/transform.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/shader.hpp>

namespace Game {

class Entity {
public:
    using MeshInstance = std::unique_ptr<rf::Mesh>;

private:
    MeshInstance m_mesh;
    rf::Transform m_transform;

public:
    Entity(MeshInstance&& mesh, const rf::Transform& transform)
        : m_mesh(std::move(mesh))
        , m_transform(transform)
    {}

public:
    void draw(const rf::Shader& shader) const {
        shader.transform(m_transform);
        shader.draw(*m_mesh);
    }

    const MeshInstance& mesh() const {
        return m_mesh;
    }

    MeshInstance& mesh() {
        return m_mesh;
    }

    const rf::Transform& transform() const {
        return m_transform;
    }

    rf::Transform& transform() {
        return m_transform;
    }
};

} // namespace Game
