#pragma once

#include <cstdint>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#include "rf/core/error.hpp"

namespace rf {

template <typename AssetType>
class AssetManager {
private:
    using Id = uint32_t;

    struct Asset {
        Id id;
        fs::path path;
        AssetType asset;
    };

public:
    class Handle {
    private:
        AssetManager* m_root = nullptr;
        Id m_id = 0;

    public:
        Handle() = default;

        Handle(AssetManager* root, Id id)
            : m_root(root)
            , m_id(id)
        {}

    public:
        bool valid() const {
            return m_root && m_id;
        }

        const AssetType* get() const {
            if (!m_root)
                throw RF_LOCATED_ERROR("Invalid asset handle is used");
            return &m_root->get(m_id).asset;
        }

    public:
        operator bool() const {
            return valid();
        }

        const AssetType* operator->() const {
            return get();
        }
    };

private:
    fs::path m_directory;
    std::unordered_map<Id, Asset> m_assets;

public:
    AssetManager(const fs::path& directory)
        : m_directory(fs::canonical(directory))
    {}

    AssetManager(const AssetManager& other) = delete;
    
    AssetManager(AssetManager&& other) noexcept
        : m_directory(std::move(other.m_directory))
        , m_assets(std::move(other.m_assets))
    {}

    ~AssetManager() = default;

public:
    AssetManager& operator=(const AssetManager& other) = delete;

    AssetManager& operator=(AssetManager&& other) noexcept {
        m_directory = std::move(other.m_directory);
        m_assets = std::move(other.m_assets);
    }

private:
    Id nextId() const {
        static Id id = 0;
        return ++id;
    }

private:
    const Asset& get(Id id) const {
        auto entry = m_assets.find(id);
        if (entry == m_assets.end())
            throw RF_LOCATED_ERROR("Unknown asset id: {}", id);
        return entry->second;
    }

public:
    Handle load(fs::path path) {
        path = path.generic_string();
        auto entry = std::find_if(
            m_assets.begin(), m_assets.end(),
            [&path](const auto& entry) {
                return entry.second.path == path;
            }
        );

        if (entry == m_assets.end()) {
            Id id = nextId();
            entry = m_assets.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(
                    id, path,
                    fs::canonical(m_directory / path)
                )
            ).first;
        }
        return { this, entry->second.id };
    }

};

} // namespace rf
