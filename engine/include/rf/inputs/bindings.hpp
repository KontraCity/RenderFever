#pragma once

#include <cstdint>
#include <string>

#include "rf/core/dispatcher.hpp"
#include "rf/inputs/actions.hpp"

namespace rf {

template <typename... Arguments>
struct Binding {
    using Id = uint64_t;
    using Handle = int64_t;
    using Dispatcher = Dispatcher<Arguments...>;

    Id id;
    Handle handle;
    std::string description;
    Dispatcher::Pointer dispatcher;
};

using KeyBinding = Binding<KeyAction>;
using CursorMoveBinding = Binding<CursorMoveAction>;
using ScrollBinding = Binding<ScrollAction>;

} // namespace rf
