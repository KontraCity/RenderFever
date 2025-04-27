#pragma once

#include <variant>

namespace rf {

namespace Input {
    enum class KeyEvent {
        None,
        Press,
        Release,
        Repeat,
    };

    const char* KeyEventName(KeyEvent event);

    KeyEvent GlfwMacroToKeyEvent(int event);

    int KeyEventToGlfwMacro(KeyEvent event);

    struct CursorMoveEvent {
        double xPosition;
        double yPosition;
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
        inline Type type() const {
            return static_cast<Type>(index());
        }
    };
}

} // namespace rf
