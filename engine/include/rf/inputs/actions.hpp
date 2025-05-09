#pragma once

#include <array>
#include <type_traits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rf/core/map_generators.hpp"

namespace rf {

enum class KeyAction {
    None        = 0,
    Press       = 1 << 0,
    Hold        = 1 << 1,
    Repeat      = 1 << 2,
    Release     = 1 << 3,
};

inline KeyAction operator|(KeyAction left, KeyAction right) {
    using KeyActionInteger = std::underlying_type<KeyAction>::type;
    auto integerLeft = static_cast<KeyActionInteger>(left);
    auto integerRight = static_cast<KeyActionInteger>(right);
    return static_cast<KeyAction>(integerLeft | integerRight);
}

inline KeyAction operator&(KeyAction left, KeyAction right) {
    using KeyActionInteger = std::underlying_type<KeyAction>::type;
    auto integerLeft = static_cast<KeyActionInteger>(left);
    auto integerRight = static_cast<KeyActionInteger>(right);
    return static_cast<KeyAction>(integerLeft & integerRight);
}

inline bool operator!(KeyAction action) {
    return action == KeyAction::None;
}

struct KeyActionEntry {
    KeyAction action;
    int macro;
    const char* name;
};

constexpr auto KeyActionRecords = std::to_array<KeyActionEntry>({
    { KeyAction::None,         -1,             "None"       },
    { KeyAction::Press,        GLFW_PRESS,     "Press"      },
    { KeyAction::Hold,         -2,             "Hold"       },
    { KeyAction::Repeat,       GLFW_REPEAT,    "Repeat"     },
    { KeyAction::Release,      GLFW_RELEASE,   "Release"    },
});

inline KeyActionEntry GetKeyActionEntry(KeyAction action) {
    constexpr auto extractor = [](KeyActionEntry entry) { return entry.action; };
    return GenerateValueToEntryMap<KeyActionRecords, KeyAction, KeyActionEntry>(extractor).at(action);
}

inline KeyActionEntry GetKeyActionEntry(int macro) {
    constexpr auto extractor = [](KeyActionEntry entry) { return entry.macro; };
    return GenerateValueToEntryMap<KeyActionRecords, int, KeyActionEntry>(extractor).at(macro);
}

struct CursorMoveAction {
    double xOffset;
    double yOffset;
};

struct ScrollAction {
    double xOffset;
    double yOffset;
};

} // namespace rf
