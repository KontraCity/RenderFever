#include "model.hpp"

#include <algorithm>
#include <utility>

namespace rf {

Graphics::Model::Model(std::vector<MatMesh>&& meshes, std::vector<Material>&& materials)
    : m_meshes(std::move(meshes))
    , m_materials(std::move(materials)) {
    for (const MatMesh& entry : m_meshes) {
        const Mesh& mesh = entry.mesh;
        m_verticesCount += mesh.verticesCount();
        m_indicesCount += mesh.indicesCount();
        m_center += mesh.center();
        m_radius = std::max(m_radius, mesh.radius());
    }
    m_center /= m_meshes.size();
}

Graphics::Model::Model(Model&& other) noexcept
    : m_meshes(std::move(other.m_meshes))
    , m_materials(std::move(other.m_materials))
    , m_verticesCount(std::exchange(other.m_verticesCount, 0))
    , m_indicesCount(std::exchange(other.m_indicesCount, 0))
    , m_center(std::exchange(other.m_center, {}))
    , m_radius(std::exchange(other.m_radius, 0.0f))
{}

Graphics::Model& Graphics::Model::operator=(Model&& other) noexcept {
    if (&other != this) {
        m_meshes = std::move(other.m_meshes);
        m_materials = std::move(other.m_materials);
        m_verticesCount = std::exchange(other.m_verticesCount, 0);
        m_indicesCount = std::exchange(other.m_indicesCount, 0);
        m_center = std::exchange(other.m_center, {});
        m_radius = std::exchange(other.m_radius, 0.0f);
    }
    return *this;
}

} // namespace rf
