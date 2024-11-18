#pragma once

// STL modules
#include <functional>
#include <mutex>
#include <map>

namespace rf {

/* Namespace aliases and imports */
using namespace std::placeholders;

namespace Engine
{
    template <typename... Parameters>
    class Dispatcher
    {
    public:
        // Callback function called when event occurs
        using Callback = std::function<void(Parameters...)>;

    private:
        mutable std::mutex m_mutex;
        size_t m_id = 1;
        std::map<size_t, Callback> m_callbacks;

    public:
        /// @brief Broadcast event
        /// @param parameters Event parameters to broadcast
        void broadcast(Parameters... parameters) const
        {
            std::lock_guard lock(m_mutex);
            for (const auto& entry : m_callbacks)
                entry.second(parameters...);
        }

        /// @brief Subscribe to event
        /// @param callback Callback function to call on event
        /// @return Subscriber ID
        size_t subscribe(const Callback& callback)
        {
            std::lock_guard lock(m_mutex);
            m_callbacks.emplace(m_id, callback);
            return m_id++;
        }

        /// @brief Unsubscribe from event
        /// @param id Subscriber ID
        void unsubscribe(size_t id)
        {
            std::lock_guard lock(m_mutex);
            m_callbacks.erase(id);
        }

        /// @brief Reset all callbacks
        void reset()
        {
            std::lock_guard lock(m_mutex);
            m_callbacks.clear();
        }
    };
}

} // namespace rf
