#pragma once

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

namespace rf {

namespace Utility {
    template <class Rep, class Period>
    inline void Sleep(std::chrono::duration<Rep, Period> duration) {
        std::this_thread::sleep_for(duration);
    }
}

} // namespace rf
