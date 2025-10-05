#include "input_map.hpp"

#include <utility>

#include <rf/core/engine.hpp>
#include <rf/graphics/window.hpp>

namespace rf {

static bool IsInputDisabled() {
    // Don't broadcast the inputs if the cursor is not disabled
    return Engine::Window().getCursorMode() != Graphics::CursorMode::Disabled;
}

Input::InputMap::InputMap(EngineCallback escapeCallback, EngineCallback captureCallback)
    : m_escapeCallback(std::move(escapeCallback))
    , m_captureCallback(std::move(captureCallback)) {
    for (Key key = Key::None; key <= Key::Mouse_Button8; ++key)
        m_keysDown[key] = false;
}

void Input::InputMap::update(float deltaTime) {
    if (IsInputDisabled())
        return;

    // We'll check the "Hold" action here
    for (const auto& keyBind : m_keyBinds) {
        if (!m_keysDown[keyBind.first] || !(keyBind.second.action & Input::KeyAction::Hold))
            continue;

        const auto bindingEntry = m_keyBindings.find(keyBind.second.handle);
        if (bindingEntry == m_keyBindings.end())
            return; // There is a bind but no binding?? Can't broadcast!
        bindingEntry->second.dispatcher->broadcast(Input::KeyAction::Hold, deltaTime);
    }
}

void Input::InputMap::broadcastKeyEvent(int glfwKey, int glfwAction) {
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

    std::lock_guard lock(m_mutex);
    if (action == KeyAction::Press) {
        if (key == Key::Key_Escape)
            m_escapeCallback();
        else if (key == Key::Mouse_Button1 || key == Key::Mouse_Button2 || key == Key::Mouse_Button3)
            m_captureCallback();
    }

    if (IsInputDisabled())
        return;

    const auto bindEntry = m_keyBinds.find(key);
    if (bindEntry == m_keyBinds.end())
        return; // This key is not bound. Broadcast not needed.
    if (!(action & bindEntry->second.action))
        return; // This key is not bound to this action. Broadcast not needed.

    const auto bindingEntry = m_keyBindings.find(bindEntry->second.handle);
    if (bindingEntry == m_keyBindings.end())
        return; // There is a bind but no binding?? Can't broadcast!
    bindingEntry->second.dispatcher->broadcast(action, 0.0f);
}

void Input::InputMap::broadcastCursorMoveEvent(double xPosition, double yPosition) const {
    if (IsInputDisabled())
        return;

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

void Input::InputMap::broadcastCursorScrollEvent(double xOffset, double yOffset) const {
    if (IsInputDisabled())
        return;

    std::lock_guard lock(m_mutex);
    m_cursorScrollDispatcher->broadcast({ xOffset, yOffset });
}

const Input::KeyBinding* Input::InputMap::getKeyBinding(KeyBindingHandle handle) const {
    std::unique_lock lock(m_mutex);
    const auto entry = m_keyBindings.find(handle);
    return entry == m_keyBindings.end() ? nullptr : &entry->second;
}

void Input::InputMap::removeKeyBinding(KeyBindingHandle handle) {
    std::lock_guard lock(m_mutex);
    m_keyBindings.erase(handle);

    // Get rid of all binds to this binding
    while (true) {
        const auto entry = std::find_if(
            m_keyBinds.begin(), m_keyBinds.end(),
            [handle](const auto& entry) { return entry.second.handle == handle; }
        );

        if (entry == m_keyBinds.end())
            break;
        m_keyBinds.erase(entry);
    }
}

void Input::InputMap::bindKey(Key key, KeyAction action, KeyBindingHandle handle) {
    std::lock_guard lock(m_mutex);
    if (m_keyBindings.contains(handle))
        m_keyBinds[key] = { action, handle };
}

void Input::InputMap::unbindKey(Key key) {
    std::lock_guard lock(m_mutex);
    m_keyBinds.erase(key);
}

void Input::InputMap::resetLastCursorPosition() {
    std::lock_guard lock(m_mutex);
    m_resetLastCursorPosition = true;
}

} // namespace rf
