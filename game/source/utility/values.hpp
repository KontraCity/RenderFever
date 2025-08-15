#pragma once

namespace Game {

namespace Utility {
    template <typename Type>
    void Limit(Type& value, Type min, Type max) {
        if (value <= min)
            value = min;
        if (value >= max)
            value = max;
    }

    template <typename Type>
    Type LimitedValue(Type value, Type min, Type max) {
        Limit(value);
        return value;
    }
}

} // namespace Game
