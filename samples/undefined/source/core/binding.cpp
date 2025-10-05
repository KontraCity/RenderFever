#include "binding.hpp"

#include <rf/core/engine.hpp>
#include <rf/input/input_map.hpp>

namespace Undefined {

rf::Input::KeyBindingCallbackHandle Bind(Binding binding, rf::Input::KeyAction action, const rf::Input::KeyBindingCallback& callback) {
    rf::Input::InputMap& inputInputMap = rf::Engine::InputMap();
    BindingEntry entry = GetBindingEntry(binding);

    rf::Input::KeyBinding* keyBinding = inputInputMap.createKeyBinding(binding, entry.description);
    inputInputMap.bindKey(entry.key, action, keyBinding->handle);
    return keyBinding->dispatcher->subscribe(callback);
}

} // namespace Undefined
