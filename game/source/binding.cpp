#include "binding.hpp"

#include <string>
#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/inputs/map.hpp>

namespace Game {

const char* Binding::TypeToDescription(Type type) {
    switch (type) {
        case Binding::Type::MoveForward:    return "Move forward";
        case Binding::Type::MoveBackward:   return "Move backward";
        case Binding::Type::MoveLeft:       return "Move left";
        case Binding::Type::MoveRight:      return "Move right";
        case Binding::Type::MoveUp:         return "Move up";
        case Binding::Type::MoveDown:       return "Move down";
        case Binding::Type::MoveSlowly:     return "Move slowly";
        case Binding::Type::MoveQuickly:    return "Move quickly";
        case Binding::Type::Action:         return "Make an action";
        case Binding::Type::Escape:         return "Get cursor or close game window";
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
        case Binding::Type::Action:         return rf::Inputs::Input::Mouse_Button1;
        case Binding::Type::Escape:         return rf::Inputs::Input::Key_Escape;
        case Binding::Type::CursorMove:     return rf::Inputs::Input::Special_CursorMove;
        case Binding::Type::Scroll:         return rf::Inputs::Input::Special_Scroll;
        case Binding::Type::ResetPlayer:    return rf::Inputs::Input::Key_R;
        default:                            return rf::Inputs::Input::None;
    }
}

void Binding::PrintBindings() {
    struct Row {
        std::string key;
        std::string description;
    };
    std::vector<Row> rows;

    const rf::Inputs::Map& inputMap = rf::Engine::InputMap();
    for (const auto& bind : inputMap.binds()) {
        if (rf::Inputs::IsSpecialInput(bind.first))
            continue;
        rows.emplace_back(rf::Inputs::InputName(bind.first), inputMap.getBinding(bind.second)->description);
    }
    std::sort(rows.begin(), rows.end(), [](const Row& left, const Row& right) { return left.key < right.key; });

    size_t longestKeyLength = std::max_element(
        rows.begin(),
        rows.end(),
        [](const Row& left, const Row& right) { return left.key.length() < right.key.length(); }
    )->key.length();
    size_t longestDescriptionLength = std::max_element(
        rows.begin(),
        rows.end(),
        [](const Row& left, const Row& right) { return left.description.length() < right.description.length(); }
    )->description.length();

    fmt::print("{:^{}}\n", "Available bindings:", 3 + longestKeyLength + 2 + longestDescriptionLength + 2);
    fmt::print("┌{:─^{}}┬{:─^{}}┐\n", "", longestKeyLength + 2, "", longestDescriptionLength + 2);
    fmt::print("│ {:^{}} │ {:^{}} │\n", "Key", longestKeyLength, "Description", longestDescriptionLength);
    fmt::print("├{:─^{}}┼{:─^{}}┤\n", "", longestKeyLength + 2, "", longestDescriptionLength + 2);
    for (const Row& row : rows)
        fmt::print("│ {:<{}} │ {:<{}} │\n", row.key, longestKeyLength, row.description, longestDescriptionLength);
    fmt::print("└{:─^{}}┴{:─^{}}┘\n", "", longestKeyLength + 2, "", longestDescriptionLength + 2);
}

Binding::Binding(Type type, const rf::Inputs::Binding::Dispatcher::Callback& callback) {
    rf::Inputs::Map& inputMap = rf::Engine::InputMap();
    rf::Inputs::Binding& binding = inputMap.createBinding(type, TypeToDescription(type));
    inputMap.bind(TypeToInput(type), binding.handle);
    m_handle = binding.dispatcher->subscribe(callback);
}

} // namespace Game
