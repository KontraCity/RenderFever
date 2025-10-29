#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <utility>

#include <rf/auxiliary/fs.hpp>

#include <rf/core/error.hpp>

namespace rf {

namespace Resources {
    template <typename Resource>
    class Manager : public std::enable_shared_from_this<Manager<Resource>> {
    public:
        using ResourceId = int64_t;

        struct ResourceEntry {
            ResourceId id;
            fs::path path;
            Resource resource;
        };

        using Resources = std::unordered_map<ResourceId, ResourceEntry>;

        class Pointer : public std::shared_ptr<Manager<Resource>> {
        private:
            using Base = std::shared_ptr<Manager<Resource>>;

        public:
            // std::make_shared requires public constructor, hence here operator new is used instead
            Pointer() : Base(new Manager()) {}
            using Base::Base;
        };

        class Handle {
        private:
            std::shared_ptr<Manager> m_root;
            ResourceId m_resourceId = 0;
            std::shared_ptr<Resource> m_standalone;

        public:
            Handle() = default;

            Handle(const std::shared_ptr<Manager>& root, ResourceId resourceId)
                : m_root(root)
                , m_resourceId(resourceId)
                , m_standalone()
            {}

            Handle(const std::shared_ptr<Resource>& standalone)
                : m_root()
                , m_resourceId(0)
                , m_standalone(standalone)
            {}

            Handle(const Handle& other) = default;

            Handle(Handle&& other) noexcept
                : m_root(std::move(other.m_root))
                , m_resourceId(std::exchange(other.m_resourceId, 0))
                , m_standalone(std::move(other.m_standalone))
            {}

            ~Handle() = default;

        public:
            Handle& operator=(const Handle& other) = default;

            Handle& operator=(Handle&& other) noexcept {
                if (this != &other) {
                    m_root = std::move(other.m_root);
                    m_resourceId = std::exchange(other.m_resourceId, 0);
                    m_standalone = std::move(other.m_standalone);
                }
                return *this;
            }

        public:
            bool isBound() const {
                return m_root.get() && m_resourceId;
            }

            bool isStandalone() const {
                return m_standalone.get();
            }

            bool isValid() const {
                return isBound() || isStandalone();
            }

            ResourceId id() const {
                return m_resourceId;
            }

            fs::path path() const {
                if (isBound())
                    return m_root->get(m_resourceId).path;
                if (isStandalone())
                    return {};
                throw RF_LOCATED_ERROR("Invalid resource handle is being used!");
            }

            const Resource* get() const {
                if (isBound())
                    return &m_root->get(m_resourceId).resource;
                if (isStandalone())
                    return m_standalone.get();
                throw RF_LOCATED_ERROR("Invalid resource handle is being used!");
            }

        public:
            operator bool() const {
                return isValid();
            }

            const Resource* operator->() const {
                return get();
            }

            const Resource& operator*() const {
                return *get();
            }
        };

    private:
        Resources m_resources;

    private:
        // ResourceEntry managers should only be created via Manager::Pointers!
        Manager() = default;

    public:
        Manager(const Manager& other) = delete;
    
        // Pointer should be moved instead (standalone Manager is prohibited)
        Manager(Manager&& other) = delete;

        ~Manager() = default;

    public:
        Manager& operator=(const Manager& other) = delete;

        // Pointer should be moved instead (standalone Manager is prohibited)
        Manager& operator=(Manager&& other) = delete;

    private:
        const ResourceEntry& get(ResourceId resourceId) const {
            auto entry = m_resources.find(resourceId);
            if (entry == m_resources.end())
                throw RF_LOCATED_ERROR("Unknown resource id is being accessed: {}", resourceId);
            return entry->second;
        }

    public:
        Handle get(fs::path path) {
            auto entry = std::find_if(
                m_resources.begin(), m_resources.end(),
                [&path](const auto& entry) { return entry.second.path == path; }
            );

            if (entry == m_resources.end())
                return {};
            return { this->shared_from_this(), entry->second.id };
        }

        Handle load(fs::path path, Resource&& resource) {
            static ResourceId s_nextResourceId = 1;
            ResourceId resourceId = s_nextResourceId++;

            auto entry = m_resources.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(resourceId),
                std::forward_as_tuple(resourceId, path, std::move(resource))
            ).first;
            return { this->shared_from_this(), entry->second.id };
        }

        void reload(const Handle& handle, Resource&& resource) {
            auto entry = m_resources.find(handle.id());
            if (entry == m_resources.end())
                return;
            entry->second.resource = std::move(resource);
        }
    };
}

} // namespace rf
