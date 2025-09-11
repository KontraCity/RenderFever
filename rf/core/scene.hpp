#pragma once

#include <flecs.h>

namespace rf {

class Scene {
private:
    flecs::world m_world;

public:
    Scene() = default;

public:
    void update() {
        // ... Entities update ...
    };

public:
    const flecs::world& world() const {
        return m_world;
    }

    flecs::world& world() {
        return m_world;
    }
};

} // namespace rf
