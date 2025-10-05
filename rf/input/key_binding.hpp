#pragma once

#include <cstdint>
#include <string>

#include <rf/core/dispatcher.hpp>
#include <rf/input/key_actions.hpp>

namespace rf {

namespace Input {
    using KeyBindingId = uint64_t;
    using KeyBindingHandle = int64_t;
    using KeyBindingDispatcher = Dispatcher<KeyAction, float>;
    using KeyBindingCallback = KeyBindingDispatcher::Callback;
    using KeyBindingCallbackHandle = KeyBindingDispatcher::Handle;

    struct KeyBinding {
        KeyBindingId id;
        KeyBindingHandle handle;
        std::string description;
        KeyBindingDispatcher::Pointer dispatcher;
    };
}

} // namespace rf
