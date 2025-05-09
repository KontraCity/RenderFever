#pragma once

#include <cstdint>
#include <string>

#include "rf/core/dispatcher.hpp"
#include "rf/inputs/actions.hpp"

namespace rf {

struct KeyBinding {
    using Id = uint64_t;
    using Handle = int64_t;
    using Dispatcher = Dispatcher<KeyAction>;

    Id id;
    Handle handle;
    std::string description;
    Dispatcher::Pointer dispatcher;
};

} // namespace rf
