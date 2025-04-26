#pragma once

#include <string>
#include <cstdint>

#include "rf/core/dispatcher.hpp"
#include "rf/input/action.hpp"

namespace rf {

namespace Input {
    struct Binding {
        using Handle = int64_t;
        using Id = uint64_t;
        using Dispatcher = Dispatcher<Action>;

        Handle handle;
        std::string name;
        Id id;
        Dispatcher dispatcher;
    };
}

} // namespace rf
