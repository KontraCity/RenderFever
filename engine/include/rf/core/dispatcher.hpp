#pragma once

#include <cstdint>
#include <functional>
#include <mutex>
#include <map>
#include <memory>
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
            : m_root(other.m_root)
            , m_callbackId(other.m_callbackId) {
            other.reset();
        }

        ~Handle() {
            auto dispatcher = m_root.lock();
            if (dispatcher && m_callbackId)
                dispatcher->unsubscribe(m_callbackId);
        }

    public:
        Handle& operator=(const Handle& other) = delete;

        Handle& operator=(Handle&& other) noexcept {
            m_root = other.m_root;
            m_callbackId = other.m_callbackId;
            other.reset();
            return *this;
        }

    private:
        void reset() {
            m_root.reset();
            m_callbackId = 0;
        }
    };

private:
    mutable std::mutex m_mutex;
    CallbackId m_nextCallbackId = 1;
    std::map<CallbackId, Callback> m_callbacks;

private:
    // Dispatchers should only be created via Dispatcher::Pointers!
    Dispatcher() = default;

public:
    Dispatcher(const Dispatcher& other) = delete;

    Dispatcher(Dispatcher&& other) = delete;

    ~Dispatcher() = default;

public:
    Dispatcher& operator=(const Dispatcher& other) = delete;

    Dispatcher& operator=(Dispatcher&& other) = delete;

private:
    inline void unsubscribe(CallbackId callbackId) {
        std::lock_guard lock(m_mutex);
        m_callbacks.erase(callbackId);
    }

public:
    inline void broadcast(Arguments... parameters) const {
        std::lock_guard lock(m_mutex);
        for (const auto& entry : m_callbacks)
            entry.second(parameters...);
    }

    [[nodiscard("Dispatcher handle must be stored while callback is in use!")]]
    inline Handle subscribe(const Callback& callback) {
        std::lock_guard lock(m_mutex);
        m_callbacks.emplace(m_nextCallbackId, callback);
        return { this->weak_from_this(), m_nextCallbackId++};
    }
};

} // namespace rf
