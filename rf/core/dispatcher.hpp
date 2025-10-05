#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <functional>
using namespace std::placeholders;

namespace rf {

template <typename... Arguments>
class Dispatcher : public std::enable_shared_from_this<Dispatcher<Arguments...>> {
public:
    using CallbackId = uint64_t;
    using Callback = std::function<void(Arguments...)>;

    class Pointer : public std::shared_ptr<Dispatcher<Arguments...>> {
    private:
        using Base = std::shared_ptr<Dispatcher<Arguments...>>;

    public:
        // std::make_shared requires public constructor, hence here operator new is used instead
        Pointer() : Base(new Dispatcher) {}
        using Base::Base;
    };

    class Handle {
    private:
        std::weak_ptr<Dispatcher> m_root;
        CallbackId m_callbackId = 0;

    public:
        Handle() = default;

        Handle(const std::weak_ptr<Dispatcher>& root, CallbackId callbackId)
            : m_root(root)
            , m_callbackId(callbackId)
        {}

        Handle(const Handle& other) = delete;

        Handle(Handle&& other) noexcept
            : m_root(std::move(other.m_root))
            , m_callbackId(std::exchange(other.m_callbackId, 0))
        {}

        ~Handle() {
            free();
        }

    public:
        Handle& operator=(const Handle& other) = delete;

        Handle& operator=(Handle&& other) noexcept {
            if (this != &other) {
                free();
                m_root = std::move(other.m_root);
                m_callbackId = std::exchange(other.m_callbackId, 0);
            }
            return *this;
        }

    private:
        void free() {
            if (!m_callbackId)
                return;

            auto dispatcher = m_root.lock();
            if (dispatcher)
                dispatcher->unsubscribe(m_callbackId);
        }
    };

private:
    mutable std::mutex m_mutex;
    std::unordered_map<CallbackId, Callback> m_callbacks;

private:
    // Dispatchers should only be created via Dispatcher::Pointers!
    Dispatcher() = default;

public:
    Dispatcher(const Dispatcher& other) = delete;

    // Pointer should be moved instead (standalone Dispatcher is prohibited)
    Dispatcher(Dispatcher&& other) = delete;

    ~Dispatcher() = default;

public:
    Dispatcher& operator=(const Dispatcher& other) = delete;

    // Pointer should be moved instead (standalone Dispatcher is prohibited)
    Dispatcher& operator=(Dispatcher&& other) = delete;

private:
    void unsubscribe(CallbackId callbackId) {
        std::lock_guard lock(m_mutex);
        m_callbacks.erase(callbackId);
    }

public:
    void broadcast(Arguments... parameters) const {
        std::lock_guard lock(m_mutex);
        for (const auto& entry : m_callbacks)
            entry.second(parameters...);
    }

    [[nodiscard("Dispatcher handle must be stored while callback is in use!")]]
    Handle subscribe(const Callback& callback) {
        static CallbackId nextCallbackid = 1;
        CallbackId callbackId = nextCallbackid++;

        std::lock_guard lock(m_mutex);
        m_callbacks.emplace(callbackId, callback);
        return { this->weak_from_this(), callbackId };
    }
};

} // namespace rf
