#pragma once

#include <array>
#include <type_traits>

#include <rf/core/map_generators.hpp>
#include <rf/input/keys.hpp>
#include <rf/input/key_actions.hpp>
#include <rf/input/key_binding.hpp>

namespace Undefined {

enum class Binding {
    None,
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    MoveQuickly,
    MoveSlowly,
    ResetPlayer,
    SpawnLight,
    ToggleVSync,
    ToggleWireframeMode,
    SwitchProjectionMode,
    SwitchToStandaloneCamera,
};

struct BindingEntry {
    Binding binding         = Binding::None;
    rf::Input::Key key      = rf::Input::Key::None;
    const char* description = nullptr;
};

constexpr auto BindingRecords = std::to_array<BindingEntry>({
    { Binding::None,                        rf::Input::Key::None,               "None"                          },
    { Binding::MoveForward,                 rf::Input::Key::Key_W,              "Move forward"                  },
    { Binding::MoveBackward,                rf::Input::Key::Key_S,              "Move backward"                 },
    { Binding::MoveLeft,                    rf::Input::Key::Key_A,              "Move left"                     },
    { Binding::MoveRight,                   rf::Input::Key::Key_D,              "Move right"                    },
    { Binding::MoveUp,                      rf::Input::Key::Key_Space,          "Move up"                       },
    { Binding::MoveDown,                    rf::Input::Key::Key_LeftControl,    "Move down"                     },
    { Binding::MoveQuickly,                 rf::Input::Key::Key_LeftShift,      "Move quickly"                  },
    { Binding::MoveSlowly,                  rf::Input::Key::Key_LeftAlt,        "Move slowly"                   },
    { Binding::ResetPlayer,                 rf::Input::Key::Key_R,              "Reset player"                  },
    { Binding::SpawnLight,                  rf::Input::Key::Key_T,              "Spawn light"                   },
    { Binding::ToggleVSync,                 rf::Input::Key::Key_I,              "Toggle VSync"                  },
    { Binding::ToggleWireframeMode,         rf::Input::Key::Key_O,              "Toggle wireframe mode"         },
    { Binding::SwitchProjectionMode,        rf::Input::Key::Key_P,              "Switch camera projection mode" },
    { Binding::SwitchToStandaloneCamera,    rf::Input::Key::Key_L,              "Switch to standalone camera"   },
});

inline BindingEntry GetBindingEntry(Binding binding) {
    return rf::GenerateValueToEntryMap<BindingRecords, Binding, BindingEntry>(
        [](const BindingEntry& entry) { return entry.binding; }
    ).at(binding);
}

inline BindingEntry GetBindingEntry(rf::Input::Key key) {
    return rf::GenerateValueToEntryMap<BindingRecords, rf::Input::Key, BindingEntry>(
        [](const BindingEntry& entry) { return entry.key; }
    ).at(key);
}

rf::Input::KeyBindingCallbackHandle Bind(Binding binding, rf::Input::KeyAction action, const rf::Input::KeyBindingCallback& callback);

} // namespace Undefined
