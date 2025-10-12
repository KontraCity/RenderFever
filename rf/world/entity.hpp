#pragma once

#include <utility>

#include <flecs.h>

namespace rf {

namespace World {
    using EntityId = flecs::entity_t;

    class Entity {
    private:
        flecs::entity m_entity;

    public:
        Entity(flecs::world& world, const char* name)
            : m_entity(world.entity(name))
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

        const char* name() const {
            return m_entity.name().c_str();
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
        ComponentType* getComponent() {
            return m_entity.get_mut<ComponentType>();
        }

        template <typename ComponentType>
        flecs::ref<ComponentType> getComponentRef() {
            return m_entity.get_ref<ComponentType>();
        }

        template <typename Function>
        void each(Function&& function) const {
            m_entity.each(std::forward<Function>(function));
        }

        template <typename Function>
        void each(Function&& function) {
            m_entity.each(std::forward<Function>(function));
        }
    };
}

} // namespace rf
