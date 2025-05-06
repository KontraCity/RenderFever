#include "binding.hpp"

#include <rf/core/engine.hpp>
#include <rf/inputs/map.hpp>

namespace Game {

const char* Binding::TypeToName(Type type) {
    switch (type) {
        case Binding::Type::MoveForward:    return "Move forward";
        case Binding::Type::MoveBackward:   return "Move backward";
        case Binding::Type::MoveLeft:       return "Move left";
        case Binding::Type::MoveRight:      return "Move right";
        case Binding::Type::MoveUp:         return "Move up";
        case Binding::Type::MoveDown:       return "Move down";
        case Binding::Type::MoveSlowly:     return "Move slowly";
        case Binding::Type::MoveQuickly:    return "Move quickly";
        case Binding::Type::CloseWindow:    return "Close game window";
        case Binding::Type::CursorMove:     return "Mouse cursor move";
        case Binding::Type::Scroll:         return "Mouse scroll";
        case Binding::Type::ResetPlayer:    return "Reset player";
        default:                            return "[Unknown binding type]";
    }
}

rf::Inputs::Input Binding::TypeToInput(Type type) {
    switch (type) {
        case Binding::Type::MoveForward:    return rf::Inputs::Input::Key_W;
        case Binding::Type::MoveBackward:   return rf::Inputs::Input::Key_S;
        case Binding::Type::MoveLeft:       return rf::Inputs::Input::Key_A;
        case Binding::Type::MoveRight:      return rf::Inputs::Input::Key_D;
        case Binding::Type::MoveUp:         return rf::Inputs::Input::Key_Space;
        case Binding::Type::MoveDown:       return rf::Inputs::Input::Key_LeftControl;
        case Binding::Type::MoveSlowly:     return rf::Inputs::Input::Key_LeftAlt;
        case Binding::Type::MoveQuickly:    return rf::Inputs::Input::Key_LeftShift;
        case Binding::Type::CloseWindow:    return rf::Inputs::Input::Key_Escape;
        case Binding::Type::CursorMove:     return rf::Inputs::Input::Special_CursorMove;
        case Binding::Type::Scroll:         return rf::Inputs::Input::Special_Scroll;
        case Binding::Type::ResetPlayer:    return rf::Inputs::Input::Key_R;
        default:                            return rf::Inputs::Input::None;
    }
}

Binding::Binding(Type type, const rf::Inputs::Binding::Dispatcher::Callback& callback) {
    rf::Inputs::Map& inputMap = rf::Engine::InputMap();
    rf::Inputs::Binding& binding = inputMap.createBinding(type, TypeToName(type));
    inputMap.bind(TypeToInput(type), binding.handle);
    m_handle = binding.dispatcher->subscribe(callback);
}

} // namespace Game
