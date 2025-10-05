#pragma once

#include <random>

namespace Undefined {

namespace Utility {
    inline int Random(int min, int max) {
        static std::mt19937 generator(std::random_device{}());
        return std::uniform_int_distribution(min, max)(generator);
    }

    inline double Random(double min, double max) {
        static std::mt19937 generator(std::random_device{}());
        return std::uniform_real_distribution(min, max)(generator);
    }
}

} // namespace Undefined
