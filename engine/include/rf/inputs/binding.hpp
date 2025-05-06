#pragma once

#include <string>
#include <cstdint>

#include "rf/core/dispatcher.hpp"
#include "rf/inputs/event.hpp"

namespace rf {

namespace Inputs {
    struct Binding {
        using Handle = int64_t;
        using Id = uint64_t;
        using Dispatcher = Dispatcher<const Event&>;

        Handle handle;
        std::string name;
        Id id;
        Dispatcher::Pointer dispatcher;
    };
}

} // namespace rf
