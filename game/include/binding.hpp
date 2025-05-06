#pragma once

#include <rf/inputs/binding.hpp>
#include <rf/inputs/input.hpp>

namespace Game {

class Binding {
public:
    enum class Type {
        MoveForward,
        MoveBackward,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveSlowly,
        MoveQuickly,

        CloseWindow,
        CursorMove,
        Scroll,
        ResetPlayer,
    };

public:
    static const char* TypeToName(Type type);

    static rf::Inputs::Input TypeToInput(Type type);

private:
    rf::Inputs::Binding::Dispatcher::Handle m_handle;

public:
    Binding() = default;

    Binding(Type type, const rf::Inputs::Binding::Dispatcher::Callback& callback);
};

} // namespace Game
