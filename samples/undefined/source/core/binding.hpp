#pragma once

#include <array>
#include <type_traits>

#include <rf/core/map_generators.hpp>
#include <rf/inputs/actions.hpp>
#include <rf/inputs/bindings.hpp>
#include <rf/inputs/keys.hpp>

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
    Action,
    Escape,
    ResetPlayer,
    ToggleVSync,
    ToggleWireframeMode,
    SwitchProjectionMode,
};

struct BindingEntry {
    Binding binding;
    rf::Key key;
    const char* name;
};

constexpr auto BindingRecords = std::to_array<BindingEntry>({
    { Binding::None,                    rf::Key::None,              "None"                                  },
    { Binding::MoveForward,             rf::Key::Key_W,             "Move forward"                          },
    { Binding::MoveBackward,            rf::Key::Key_S,             "Move backward"                         },
    { Binding::MoveLeft,                rf::Key::Key_A,             "Move left"                             },
    { Binding::MoveRight,               rf::Key::Key_D,             "Move right"                            },
    { Binding::MoveUp,                  rf::Key::Key_Space,         "Move up"                               },
    { Binding::MoveDown,                rf::Key::Key_LeftControl,   "Move down"                             },
    { Binding::MoveQuickly,             rf::Key::Key_LeftShift,     "Move quickly"                          },
    { Binding::MoveSlowly,              rf::Key::Key_LeftAlt,       "Move slowly"                           },
    { Binding::Action,                  rf::Key::Mouse_Button1,     "Capture cursor"                        },
    { Binding::Escape,                  rf::Key::Key_Escape,        "Release cursor / close game window"    },
    { Binding::ResetPlayer,             rf::Key::Key_R,             "Reset player"                          },
    { Binding::ToggleVSync,             rf::Key::Key_I,             "Toggle vertical synchronization"       },
    { Binding::ToggleWireframeMode,     rf::Key::Key_O,             "Toggle wireframe mode"                 },
    { Binding::SwitchProjectionMode,    rf::Key::Key_P,             "Switch camera projection mode"         },
});

inline BindingEntry GetBindingEntry(Binding binding) {
    constexpr auto extractor = [](BindingEntry entry) { return entry.binding; };
    return rf::GenerateValueToEntryMap<BindingRecords, Binding, BindingEntry>(extractor).at(binding);
}

inline BindingEntry GetBindingEntry(rf::Key key) {
    constexpr auto extractor = [](BindingEntry entry) { return entry.key; };
    return rf::GenerateValueToEntryMap<BindingRecords, rf::Key, BindingEntry>(extractor).at(key);
}

void ShowBindings();

rf::KeyBinding::Dispatcher::Handle Bind(Binding binding, rf::KeyAction action, const rf::KeyBinding::Dispatcher::Callback& callback);

} // namespace Undefined
