#pragma once

#include <unordered_map>
#include <algorithm>
#include <utility>

#include <flecs.h>

#include <rf/graphics/camera.hpp>
#include <rf/world/components.hpp>
#include <rf/world/entity.hpp>

namespace rf {

namespace World {
    class Scene {
    public:
        using Entities = std::unordered_map<EntityId, Entity>;

    private:
        flecs::world m_world;
        Entities m_entities;

    public:
        Scene() = default;

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
            m_world.each([this](flecs::entity entity, const LogicComponent& component) {
                if (!component.onStart)
                    return;
                component.onStart(m_entities.at(entity.id()), 0.0f);
            });
        }

        void update(float deltaTime) {
            m_world.each([this, deltaTime](flecs::entity entity, const LogicComponent& component) {
                if (!component.onUpdate)
                    return;
                component.onUpdate(m_entities.at(entity.id()), deltaTime);
            });
        }

    public:
        const Entities& entities() const {
            return m_entities;
        }

        Entities& entities() {
            return m_entities;
        }

        Entity& newEntity(const char* name = "Entity") {
            for (size_t index = 0; true; ++index) {
                std::string postfixedName = index == 0 ? name : fmt::format("{} ({})", name, index);
                auto entry = std::find_if(m_entities.begin(), m_entities.end(), [&postfixedName](const auto& entry) {
                    return postfixedName == entry.second.name();
                });

                if (entry == m_entities.end()) {
                    Entity entity(m_world, postfixedName.c_str());
                    return m_entities.emplace(entity.id(), std::move(entity)).first->second;
                }
            }
        }

        const Entity* getEntity(EntityId id) const {
            auto entry = m_entities.find(id);
            return entry == m_entities.end() ? nullptr : &entry->second;
        }

        Entity* getEntity(EntityId id) {
            auto entry = m_entities.find(id);
            return entry == m_entities.end() ? nullptr : &entry->second;
        }

        const Entity* getActiveCameraEntity() const {
            flecs::entity entity = m_world.query<CameraComponent, CameraComponent::ActiveCameraTag>().first();
            return entity.is_alive() ? &m_entities.at(entity.id()) : nullptr;
        }

        Entity* getActiveCameraEntity() {
            flecs::entity entity = m_world.query<CameraComponent, CameraComponent::ActiveCameraTag>().first();
            return entity.is_alive() ? &m_entities.at(entity.id()) : nullptr;
        }

        void setActiveCameraEntity(Entity& entity) {
            // Don't do anything if this entity doesn't have a camera.
            if (!entity.has<CameraComponent>())
                return;

            // Ensure that no entities have active camera tag and give the tag to this entity.
            m_world.remove_all<CameraComponent::ActiveCameraTag>();
            entity.add<CameraComponent::ActiveCameraTag>();
        }

        const Graphics::Camera* getActiveCamera() const {
            const rf::World::Entity* cameraEntity = getActiveCameraEntity();
            if (!cameraEntity)
                return nullptr;

            const CameraComponent* cameraComponent = cameraEntity->get<rf::World::CameraComponent>();
            return cameraComponent ? &cameraComponent->camera : nullptr;
        }

        Graphics::Camera* getActiveCamera() {
            rf::World::Entity* cameraEntity = getActiveCameraEntity();
            if (!cameraEntity)
                return nullptr;

            CameraComponent* cameraComponent = cameraEntity->get<rf::World::CameraComponent>();
            return cameraComponent ? &cameraComponent->camera : nullptr;
        }

        template <typename Component>
        bool is(flecs::id id) {
            return id == m_world.id<Component>();
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
        Component* get() {
            return m_world.singleton<Component>().get_mut<Component>();
        }

        template <typename... Components>
        flecs::query<Components...> query() const {
            return m_world.query<Components...>();
        }

        template <typename... Components>
        flecs::query<Components...> query() {
            return m_world.query<Components...>();
        }

        template <typename Function>
        void each(Function&& function) const {
            m_world.each(std::forward<Function>(function));
        }

        template <typename Function>
        void each(Function&& function) {
            m_world.each(std::forward<Function>(function));
        }
    };
}

} // namespace rf
