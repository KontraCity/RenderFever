#pragma once

#include <unordered_map>
#include <utility>

#include <flecs.h>

#include <rf/graphics/camera.hpp>
#include <rf/world/components.hpp>
#include <rf/world/entity.hpp>

namespace rf {

namespace World {
    class Scene {
    private:
        std::unordered_map<flecs::entity_t, Entity> m_entities;
        flecs::world m_world;

    public:
        Scene() {
            // TODO: A better way to do this?
            m_world.add<rf::Graphics::Camera>();
        }

        Scene(const Scene& other) = delete;

        Scene(Scene&& other) noexcept = delete;

        ~Scene() {
            // Ensure that the entities are destroyed before the world.
            m_entities.clear();
        }

    public:
        Scene& operator=(const Scene& other) = delete;

        Scene& operator=(Scene&& other) noexcept = delete;

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
            return m_entities.emplace(entity.id(), std::move(entity)).first->second;
        }

        template <typename Component>
        void add() {
            m_world.add<Component>();
        }

        template <typename Component>
        void set(Component&& value) {
            m_world.set<Component>(std::forward<Component>(value));
        }

        template <typename Component>
        const Component* get() const {
            return m_world.singleton<Component>().get<Component>();
        }

        template <typename Component>
        flecs::ref<Component> get() {
            return m_world.singleton<Component>().get_ref<Component>();
        }

        template <typename... Components>
        flecs::query<Components...> query() {
            return m_world.query<Components...>();
        }

        template <typename Function>
        void each(Function&& function) {
            m_world.each(std::forward<Function>(function));
        }
    };
}

} // namespace rf
