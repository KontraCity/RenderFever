#pragma once

#include <rf/inputs/binding.hpp>
#include <rf/inputs/input.hpp>

namespace Game {

class Binding {
public:
    enum class Type {
        None,

        MoveForward,
        MoveBackward,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveSlowly,
        MoveQuickly,

        Action,
        Escape,
        CursorMove,
        Scroll,
        ResetPlayer,
    };

public:
    static const char* TypeToDescription(Type type);

    static rf::Inputs::Input TypeToInput(Type type);

    static void PrintBindings();

private:
    rf::Inputs::Binding::Dispatcher::Handle m_handle;

public:
    Binding() = default;

    Binding(Type type, const rf::Inputs::Binding::Dispatcher::Callback& callback);
};

} // namespace Game
