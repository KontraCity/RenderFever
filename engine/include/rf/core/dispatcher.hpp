#pragma once

#include <functional>
#include <mutex>
#include <map>
using namespace std::placeholders;

namespace rf {

namespace Core {
    template <typename... Parameters>
    class Dispatcher {
    public:
        using Callback = std::function<void(Parameters...)>;

    private:
        mutable std::mutex m_mutex;
        size_t m_id = 1;
        std::map<size_t, Callback> m_callbacks;

    public:
        Dispatcher() = default;

        Dispatcher(const Dispatcher& other) = delete;

        Dispatcher(Dispatcher&& other) noexcept = delete;

    public:
        inline void broadcast(Parameters... parameters) const {
            std::lock_guard lock(m_mutex);
            for (const auto& entry : m_callbacks)
                entry.second(parameters...);
        }

        inline size_t subscribe(const Callback& callback) {
            std::lock_guard lock(m_mutex);
            m_callbacks.emplace(m_id, callback);
            return m_id++;
        }

        inline void unsubscribe(size_t id) {
            std::lock_guard lock(m_mutex);
            m_callbacks.erase(id);
        }

        inline void reset() {
            std::lock_guard lock(m_mutex);
            m_callbacks.clear();
        }
    };
}

} // namespace rf
