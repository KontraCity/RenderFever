#pragma once

#include <mutex>
#include <map>

#include "rf/core/dispatcher.hpp"
#include "rf/inputs/actions.hpp"
#include "rf/inputs/bindings.hpp"
#include "rf/inputs/keys.hpp"

namespace rf {

class InputMap {
public:
    struct ConstHandle {
        std::unique_lock<std::recursive_mutex> lock;
        const KeyBinding* binding = nullptr;
    };

    struct Handle {
        std::unique_lock<std::recursive_mutex> lock;
        KeyBinding* binding = nullptr;
    };

    struct KeyBind {
        KeyAction action;
        KeyBinding::Handle bindingHandle;
    };

    using CursorMoveDispatcher = Dispatcher<CursorMoveAction>;
    using CursorScrollDispatcher = Dispatcher<CursorScrollAction>;

private:
    mutable std::recursive_mutex m_mutex;
    std::map<Key, bool> m_keysDown;
    std::map<Key, KeyBind> m_keyBinds;
    std::map<KeyBinding::Handle, KeyBinding> m_keyBindings;
    KeyBinding::Handle m_nextKeyBindingHandle = 1;

    CursorMoveDispatcher::Pointer m_cursorMoveDispatcher;
    CursorScrollDispatcher::Pointer m_scrollDispatcher;
    mutable bool m_resetLastCursorPosition = false;

public:
    InputMap() {
        for (Key key = Key::None; key <= Key::Mouse_Button8; ++key) {
            m_keysDown[key] = false;
        }
    }

public:
    void update() {
        for (const auto& keyBind : m_keyBinds) {
            if (!m_keysDown[keyBind.first] || !(keyBind.second.action & rf::KeyAction::Hold))
                continue;

            const auto bindingEntry = m_keyBindings.find(keyBind.second.bindingHandle);
            if (bindingEntry == m_keyBindings.end())
                return; // There is a bind but no binding?? Can't broadcast!
            bindingEntry->second.dispatcher->broadcast(rf::KeyAction::Hold);
        }
    }

    void broadcastKeyEvent(int glfwKey, int glfwAction) {
        std::lock_guard lock(m_mutex);
        Key key = GetKeyEntry(glfwKey).key;
        if (key == Key::None)
            return; // This key is unknown. Can't broadcast!

        KeyAction action = GetKeyActionEntry(glfwAction).action;
        if (action == KeyAction::None)
            return; // This key action is unknown. Can't broadcast!
        else if (action == KeyAction::Press)
            m_keysDown[key] = true;
        else if (action == KeyAction::Release)
            m_keysDown[key] = false;

        const auto bindEntry = m_keyBinds.find(key);
        if (bindEntry == m_keyBinds.end())
            return; // This key is not bound. Broadcast not needed.
        if (!(action & bindEntry->second.action))
            return; // This key is not bound to this action. Broadcast not needed.

        const auto bindingEntry = m_keyBindings.find(bindEntry->second.bindingHandle);
        if (bindingEntry == m_keyBindings.end())
            return; // There is a bind but no binding?? Can't broadcast!
        bindingEntry->second.dispatcher->broadcast(action);
    }

    void broadcastCursorMoveEvent(double xPosition, double yPosition) const {
        static float lastXPosition = xPosition;
        static float lastYPosition = yPosition;

        std::lock_guard lock(m_mutex);
        if (m_resetLastCursorPosition) {
            lastXPosition = xPosition;
            lastYPosition = yPosition;
            m_resetLastCursorPosition = false;
        }

        float xOffset = xPosition - lastXPosition;
        float yOffset = lastYPosition - yPosition;
        lastXPosition = xPosition;
        lastYPosition = yPosition;
        m_cursorMoveDispatcher->broadcast({ xOffset, yOffset });
    }

    void broadcastCursorScrollEvent(double xOffset, double yOffset) const {
        std::lock_guard lock(m_mutex);
        m_scrollDispatcher->broadcast({ xOffset, yOffset });
    }

    template <typename Id>
    Handle createKeyBinding(Id id, const std::string& description) {
        std::unique_lock lock(m_mutex);
        KeyBinding::Handle handle = m_nextKeyBindingHandle++;
        return { std::move(lock), &m_keyBindings.try_emplace(handle, static_cast<KeyBinding::Id>(id), handle, description).first->second };
    }

    template <typename Id>
    ConstHandle getKeyBinding(Id id) const {
        std::unique_lock lock(m_mutex);
        auto bindingId = static_cast<KeyBinding::Id>(id);
        const auto entry = std::find_if(
            m_keyBindings.begin(), m_keyBindings.end(),
            [bindingId](const auto& entry) { return entry.second.id == bindingId; }
        );
        return { std::move(lock), entry == m_keyBindings.end() ? nullptr : &entry->second };
    }

    ConstHandle getKeyBinding(KeyBinding::Handle handle) const {
        std::unique_lock lock(m_mutex);
        const auto entry = m_keyBindings.find(handle);
        return { std::move(lock), entry == m_keyBindings.end() ? nullptr : &entry->second };
    }

    void removeKeyBinding(KeyBinding::Handle handle) {
        std::lock_guard lock(m_mutex);
        m_keyBindings.erase(handle);

        // Get rid of all binds to this binding
        while (true) {
            const auto entry = std::find_if(
                m_keyBinds.begin(), m_keyBinds.end(),
                [handle](const auto& entry) { return entry.second.bindingHandle == handle; }
            );

            if (entry == m_keyBinds.end())
                break;
            m_keyBinds.erase(entry);
        }
    }

    void bindKey(Key key, KeyAction action, KeyBinding::Handle handle) {
        std::lock_guard lock(m_mutex);
        if (m_keyBindings.contains(handle))
            m_keyBinds[key] = { action, handle };
    }

    void unbindKey(Key key) {
        std::lock_guard lock(m_mutex);
        m_keyBinds.erase(key);
    }

    void resetLastCursorPosition() {
        std::lock_guard lock(m_mutex);
        m_resetLastCursorPosition = true;
    }

public:
    std::map<Key, KeyBind> binds() const {
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

    const CursorScrollDispatcher::Pointer& scrollDispatcher() const {
        // Dispatchers are thread safe
        return m_scrollDispatcher;
    }
};

} // namespace rf 
