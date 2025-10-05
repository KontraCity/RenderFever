#pragma once

namespace Undefined {

namespace Utility {
    template <typename Type>
    inline void Limit(Type& value, Type min, Type max) {
        if (value <= min)
            value = min;
        if (value >= max)
            value = max;
    }

    template <typename Type>
    inline Type LimitedValue(Type value, Type min, Type max) {
        Limit(value);
        return value;
    }
}

} // namespace Undefined
