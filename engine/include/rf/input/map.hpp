#pragma once

#include <mutex>
#include <map>

#include "rf/input/binding.hpp"
#include "rf/input/event.hpp"
#include "rf/input/input.hpp"

namespace rf {

namespace Input {
    class Map {
    private:
        mutable std::mutex m_mutex;
        std::map<Input, Binding::Handle> m_binds;
        std::map<Binding::Handle, Binding> m_bindings;
        Binding::Handle m_nextBindingHandle = 1;

    private:
        inline void broadcast(Input input, const Event& event) const {
            const auto bindEntry = m_binds.find(input);
            if (bindEntry == m_binds.end()) {
                // This input is not bound. Broadcast not needed.
                return;
            }

            const auto bindingEntry = m_bindings.find(bindEntry->second);
            if (bindingEntry == m_bindings.end()) {
                // There is a bind but no binding?? Can't broadcast!
                return;
            }
            bindingEntry->second.dispatcher.broadcast({ event });
        }

    public:
        inline void broadcastKeyEvent(int glfwKey, int glfwAction) const {
            std::lock_guard lock(m_mutex);
            Input input = GlfwMacroToInput(glfwKey);
            if (input == Input::None || IsSpecialInput(input)) {
                // This key is unknown or special. Can't broadcast!
                return;
            }

            KeyEvent event = GlfwMacroToKeyEvent(glfwAction);
            if (event == KeyEvent::None) {
                // This key event is unknown. Can't broadcast!
                return;
            }
            broadcast(input, { event });
        }

        inline void broadcastCursorMoveEvent(double xPosition, double yPosition) const {
            std::lock_guard lock(m_mutex);
            broadcast(Input::Special_CursorMove, CursorMoveEvent{ xPosition, yPosition });
        }

        inline void broadcastScrollEvent(double xOffset, double yOffset) const {
            std::lock_guard lock(m_mutex);
            broadcast(Input::Special_Scroll, ScrollEvent{ xOffset, yOffset });
        }

        template <typename Id>
        inline Binding& createBinding(Id id, const std::string& name) {
            std::lock_guard lock(m_mutex);
            Binding::Handle handle = m_nextBindingHandle++;
            return m_bindings.try_emplace(handle, handle, name, static_cast<Binding::Id>(id)).first->second;
        }

        inline const Binding* getBinding(Binding::Handle handle) const {
            std::lock_guard lock(m_mutex);
            const auto entry = m_bindings.find(handle);
            return entry == m_bindings.end() ? nullptr : &entry->second;
        }

        template <typename Id>
        inline const Binding* getBinding(Id id) const {
            std::lock_guard lock(m_mutex);
            Binding::Id bindingId = static_cast<Binding::Id>(id);
            const auto entry = std::find_if(
                m_bindings.begin(), m_bindings.end(),
                [bindingId](const auto& entry) { return entry.second.id == bindingId; }
            );
            return entry == m_bindings.end() ? nullptr : &entry->second;
        }

        inline void removeBinding(Binding::Handle handle) {
            std::lock_guard lock(m_mutex);
            m_bindings.erase(handle);

            // Get rid of all binds to this binding
            while (true) {
                const auto entry = std::find_if(
                    m_binds.begin(), m_binds.end(),
                    [handle](const auto& entry) { return entry.second == handle; }
                );

                if (entry == m_binds.end())
                    break;
                m_binds.erase(entry);
            }
        }

        inline void bind(Input input, Binding::Handle handle) {
            std::lock_guard lock(m_mutex);
            if (m_bindings.contains(handle))
                m_binds[input] = handle;
        }

        inline void unbind(Input input) {
            std::lock_guard lock(m_mutex);
            m_binds.erase(input);
        }
    };
}

} // namespace rf 
