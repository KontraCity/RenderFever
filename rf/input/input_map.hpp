#pragma once

#include <mutex>
#include <unordered_map>

#include <rf/auxiliary/gl.hpp>

#include <rf/core/dispatcher.hpp>
#include <rf/input/keys.hpp>
#include <rf/input/key_actions.hpp>
#include <rf/input/key_binding.hpp>

namespace rf {

namespace Input {
    class InputMap {
    public:
        struct KeyBind {
            KeyAction action;
            KeyBindingHandle handle;
        };

        using EngineCallback = std::function<void()>;
        using CursorMoveDispatcher = Dispatcher<CursorMoveAction>;
        using CursorScrollDispatcher = Dispatcher<CursorScrollAction>;

    private:
        mutable std::recursive_mutex m_mutex;
        std::unordered_map<Key, bool> m_keysDown;
        std::unordered_map<Key, KeyBind> m_keyBinds;
        std::unordered_map<KeyBindingHandle, KeyBinding> m_keyBindings;

        EngineCallback m_escapeCallback;
        EngineCallback m_captureCallback;
        CursorMoveDispatcher::Pointer m_cursorMoveDispatcher;
        CursorScrollDispatcher::Pointer m_cursorScrollDispatcher;
        mutable bool m_resetLastCursorPosition = false;

    public:
        InputMap(EngineCallback escapeCallback, EngineCallback captureCallback);

        InputMap(const InputMap& other) = delete;

        InputMap(InputMap&& other) noexcept = delete;

        ~InputMap() = default;

    public:
        InputMap& operator=(const InputMap& other) = delete;

        InputMap& operator=(InputMap&& other) noexcept = delete;

    public:
        void update(float deltaTime);

        void broadcastKeyEvent(int glfwKey, int glfwAction);

        void broadcastCursorMoveEvent(double xPosition, double yPosition) const;

        void broadcastCursorScrollEvent(double xOffset, double yOffset) const;

        const KeyBinding* getKeyBinding(KeyBindingHandle handle) const;

        void removeKeyBinding(KeyBindingHandle handle);

        void bindKey(Key key, KeyAction action, KeyBindingHandle handle);

        void unbindKey(Key key);

        void resetLastCursorPosition();

    public:
        template <typename Id>
        KeyBinding* createKeyBinding(Id id, const std::string& description) {
            static KeyBindingHandle s_nextKeyBindingHandle = 1;
            KeyBindingHandle keyBindingHandle = s_nextKeyBindingHandle++;

            std::unique_lock lock(m_mutex);
            auto entry = m_keyBindings.try_emplace(
                keyBindingHandle,
                static_cast<KeyBindingId>(id),
                keyBindingHandle,
                description
            );
            return &entry.first->second;
        }

        template <typename Id>
        const KeyBinding* getKeyBinding(Id id) const {
            std::unique_lock lock(m_mutex);
            auto bindingId = static_cast<KeyBinding::Id>(id);
            const auto entry = std::find_if(
                m_keyBindings.begin(), m_keyBindings.end(),
                [bindingId](const auto& entry) { return entry.second.id == bindingId; }
            );
            return entry == m_keyBindings.end() ? nullptr : &entry->second;
        }

    public:
        std::unordered_map<Key, KeyBind> binds() const {
            std::lock_guard lock(m_mutex);
            return m_keyBinds;
        }

        bool keyDown(Key key) const {
            std::lock_guard lock(m_mutex);
            return m_keysDown.at(key);
        }

        const CursorMoveDispatcher::Pointer& cursorMoveDispatcher() const {
            // Dispatchers are thread safe
            return m_cursorMoveDispatcher;
        }

        const CursorScrollDispatcher::Pointer& cursorScrollDispatcher() const {
            // Dispatchers are thread safe
            return m_cursorScrollDispatcher;
        }
    };
}

} // namespace rf 
