#pragma once

#include <variant>

#include "rf/inputs/input.hpp"

namespace rf {

enum class KeyEvent {
    None,
    Press,
    Release,
    Repeat,
};

const char* KeyEventName(KeyEvent event);

KeyEvent GlfwMacroToKeyEvent(int event);

int KeyEventToGlfwMacro(KeyEvent event);

bool KeyPressed(Input input);

struct CursorMoveEvent {
    double xOffset;
    double yOffset;
};

struct ScrollEvent {
    double xOffset;
    double yOffset;
};

class Event : public std::variant<KeyEvent, CursorMoveEvent, ScrollEvent> {
public:
    enum class Type {
        KeyEvent,
        CursorMoveEvent,
        ScrollEvent,
    };
    
public:
    using variant::variant;

public:
    Type type() const {
        return static_cast<Type>(index());
    }
};

} // namespace rf
