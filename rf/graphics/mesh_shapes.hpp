#pragma once

#include "rf/graphics/mesh.hpp"

namespace rf {

class PlaneMesh : public Mesh {
public:
    PlaneMesh();
    using Mesh::Mesh;
};

class CubeMesh : public Mesh {
public:
    CubeMesh();
    using Mesh::Mesh;
};

} // namespace rf
