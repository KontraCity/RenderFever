#include "binding.hpp"

#include <string>
#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/inputs/input_map.hpp>

namespace Game {

void ShowBindings() {
    struct Row {
        Binding binding;
        std::string key;
        std::string description;
    };
    std::vector<Row> rows;

    const rf::InputMap& inputInputMap = rf::Engine::InputMap();
    for (const auto& bind : inputInputMap.binds()) {
        rf::InputMap::ConstHandle handle = inputInputMap.getKeyBinding(bind.second.bindingHandle);
        rows.emplace_back(static_cast<Binding>(handle.binding->id), rf::GetKeyEntry(bind.first).name, handle.binding->description);
    }
    std::sort(
        rows.begin(), rows.end(),
        [](const Row& left, const Row& right) {
            using BindingInteger = std::underlying_type<Binding>::type;
            return static_cast<BindingInteger>(left.binding) < static_cast<BindingInteger>(right.binding);
        }
    );

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
        fmt::print("│ {:^{}} │ {:<{}} │\n", row.key, longestKeyLength, row.description, longestDescriptionLength);
    fmt::print("└{:─^{}}┴{:─^{}}┘\n", "", longestKeyLength + 2, "", longestDescriptionLength + 2);
}

rf::KeyBinding::Dispatcher::Handle Bind(Binding binding, rf::KeyAction action, const rf::KeyBinding::Dispatcher::Callback& callback) {
    rf::InputMap& inputInputMap = rf::Engine::InputMap();
    BindingEntry entry = GetBindingEntry(binding);

    rf::InputMap::Handle handle = inputInputMap.createKeyBinding(binding, entry.name);
    inputInputMap.bindKey(entry.key, action, handle.binding->handle);
    return handle.binding->dispatcher->subscribe(callback);
}

} // namespace Game
