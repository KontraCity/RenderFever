#pragma once

#include <flecs.h>

#include "rf/core/assets.hpp"
#include "rf/graphics/camera.hpp"
#include "rf/graphics/material.hpp"
#include "rf/graphics/mesh.hpp"
#include "rf/graphics/shader.hpp"
#include "rf/graphics/transform.hpp"

namespace rf {

class Entity {
private:
    flecs::entity m_entity;

public:
    Entity(flecs::world& world)
        : m_entity(world.entity())
    {}

    Entity(const Entity& other) = delete;

    Entity(Entity&& other) noexcept {
        m_entity = std::move(other.m_entity);
        other.m_entity = {};
    }

    ~Entity() {
        if (m_entity.is_valid())
            m_entity.destruct();
    }

public:
    template <typename ComponentType>
    Entity& addComponent() {
        m_entity.add<ComponentType>();
        return *this;
    }

    template <typename ComponentType>
    Entity& addComponent(ComponentType&& value) {
        m_entity.add<ComponentType>(std::forward<ComponentType>(value));
        return *this;
    }

    template <typename ComponentType>
    Entity& setComponent(ComponentType&& value) {
        m_entity.set<ComponentType>(std::forward<ComponentType>(value));
        return *this;
    }

public:
    flecs::entity_t getId() const {
        return m_entity.id();
    }

    template <typename ComponentType>
    const ComponentType* getComponent() const {
        return m_entity.get<ComponentType>();
    }

    template <typename ComponentType>
    flecs::ref<ComponentType> getComponent() {
        return m_entity.get_ref<ComponentType>();
    }
};

struct CameraComponent {
    Camera camera;
};

struct DrawComponent {
    Assets::Shader shader;
    Transform transform;
    Material material;
    Mesh mesh;
};

struct StartComponent {
    using Callback = std::function<void(Entity&)>;
    Callback onStart;
};

struct UpdateComponent {
    using Callback = std::function<void(Entity&, float)>;
    Callback onUpdate;
};

} // namespace rf
