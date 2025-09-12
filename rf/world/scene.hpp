#pragma once

#include <deque>

#include <flecs.h>

#include "rf/world/entity.hpp"

namespace rf {

class Scene {
private:
    std::map<flecs::entity_t, Entity> m_entities;
    flecs::world m_world;

public:
    Scene() {
        m_world.add<rf::Camera>();
    }

    ~Scene() {
        m_entities.clear();
    }

public:
    void start() {
        m_world.each([this](flecs::entity entity, const StartComponent& component) {
            component.onStart(m_entities.at(entity.id()));
        });
    }

    void update(float deltaTime) {
        m_world.each([this, deltaTime](flecs::entity entity, const UpdateComponent& component) {
            component.onUpdate(m_entities.at(entity.id()), deltaTime);
        });
    }

public:
    Entity& newEntity() {
        Entity entity(m_world);
        return m_entities.emplace(entity.getId(), std::move(entity)).first->second;
    }

    template <typename ComponentType>
    void add() {
        m_world.add<ComponentType>();
    }

    template <typename ComponentType>
    const ComponentType* get() const {
        return m_world.singleton<ComponentType>().get<ComponentType>();
    }

    template <typename ComponentType>
    flecs::ref<ComponentType> get() {
        return m_world.singleton<ComponentType>().get_ref<ComponentType>();
    }

    template <typename Function>
    void each(Function&& function) {
        m_world.each(std::forward<Function>(function));
    }
};

} // namespace rf
