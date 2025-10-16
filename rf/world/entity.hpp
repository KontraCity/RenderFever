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

        template <typename Component>
        bool has() const {
            return m_entity.has<Component>();
        }

        template <typename Component>
        Entity& add() {
            m_entity.add<Component>();
            return *this;
        }

        template <typename Component>
        Entity& add(Component&& value) {
            m_entity.add<Component>(std::forward<Component>(value));
            return *this;
        }

        template <typename Component>
        Entity& set(Component&& value) {
            m_entity.set<Component>(std::forward<Component>(value));
            return *this;
        }

        template <typename Component>
        const Component* get() const {
            return m_entity.get<Component>();
        }

        template <typename Component>
        Component* get() {
            return m_entity.get_mut<Component>();
        }

        template <typename Component>
        void remove() {
            m_entity.remove<Component>();
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
