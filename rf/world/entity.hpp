#pragma once

#include <utility>

#include <flecs.h>

namespace rf {

namespace World {
    class Entity {
    private:
        flecs::entity m_entity;

    public:
        Entity(flecs::world& world)
            : m_entity(world.entity())
        {}

        Entity(const Entity& other) = delete;

        Entity(Entity&& other) noexcept
            : m_entity(std::exchange(other.m_entity, {}))
        {}

        ~Entity() {
            if (m_entity.is_valid())
                m_entity.destruct();
        }

    public:
        Entity& operator=(const Entity& other) = delete;

        Entity& operator=(Entity&& other) noexcept {
            if (this != &other)
                m_entity = std::exchange(other.m_entity, {});
            return *this;
        }

    public:
        flecs::entity_t id() const {
            return m_entity.id();
        }

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

        template <typename ComponentType>
        const ComponentType* getComponent() const {
            return m_entity.get<ComponentType>();
        }

        template <typename ComponentType>
        flecs::ref<ComponentType> getComponent() {
            return m_entity.get_ref<ComponentType>();
        }
    };
}

} // namespace rf
