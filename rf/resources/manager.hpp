#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <algorithm>

#include <rf/auxiliary/fs.hpp>

#include <rf/core/error.hpp>

namespace rf {

namespace Resources {
    template <typename ResourceType>
    class Manager : public std::enable_shared_from_this<Manager<ResourceType>> {
    public:
        using ResourceId = uint64_t;
        struct Resource {
            ResourceId id;
            fs::path path;
            ResourceType resource;
        };

        class Pointer : public std::shared_ptr<Manager<ResourceType>> {
        private:
            using Base = std::shared_ptr<Manager<ResourceType>>;

        public:
            // std::make_shared requires public constructor, hence here operator new is used instead
            Pointer(const fs::path& directory) : Base(new Manager(directory)) {}
            using Base::Base;
        };

        class Handle {
        private:
            std::shared_ptr<Manager> m_root;
            ResourceId m_resourceId = 0;

        public:
            Handle() = default;

            Handle(const std::shared_ptr<Manager>& root, ResourceId resourceId)
                : m_root(root)
                , m_resourceId(resourceId)
            {}

            Handle(const Handle& other) = default;

            Handle(Handle&& other) noexcept
                : m_root(std::move(other.m_root))
                , m_resourceId(std::exchange(other.m_resourceId, 0))
            {}

            ~Handle() = default;

        public:
            Handle& operator=(const Handle& other) = default;

            Handle& operator=(Handle&& other) noexcept {
                if (this != &other) {
                    m_root = std::move(other.m_root);
                    m_resourceId = std::exchange(other.m_resourceId, 0);
                }
                return *this;
            }

        public:
            bool isValid() const {
                return m_root && m_resourceId;
            }

            const ResourceType* get() const {
                if (!isValid())
                    throw RF_LOCATED_ERROR("Invalid resource handle is being used");
                return &m_root->get(m_resourceId).resource;
            }

        public:
            operator bool() const {
                return isValid();
            }

            const ResourceType* operator->() const {
                return get();
            }
        };

    private:
        mutable std::mutex m_mutex;
        fs::path m_directory;
        std::unordered_map<ResourceId, Resource> m_resources;

    private:
        // Resource managers should only be created via Manager::Pointers!
        Manager(const fs::path& directory)
            : m_directory(fs::canonical(directory))
        {}

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
        const Resource& get(ResourceId resourceId) const {
            std::lock_guard lock(m_mutex);
            auto entry = m_resources.find(resourceId);
            if (entry == m_resources.end())
                throw RF_LOCATED_ERROR("Unknown resource id is being accessed: {}", resourceId);
            return entry->second;
        }

    public:
        Handle load(fs::path path) {
            std::lock_guard lock(m_mutex);
            path = path.generic_string();
            auto entry = std::find_if(
                m_resources.begin(), m_resources.end(),
                [&path](const auto& entry) {
                    return entry.second.path == path;
                }
            );

            if (entry == m_resources.end()) {
                static ResourceId nextResourceId = 1;
                ResourceId resourceId = nextResourceId++;

                entry = m_resources.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(resourceId),
                    std::forward_as_tuple(
                        resourceId, path,
                        fs::canonical(m_directory / path)
                    )
                ).first;
            }
            return { this->shared_from_this(), entry->second.id};
        }
    };
}

} // namespace rf
