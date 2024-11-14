#pragma once

// STL modules
#include <random>

namespace rf {

namespace Utility
{
    /// @brief Generate random integer
    /// @param min Min integer value
    /// @param max Max integer value
    /// @return Generated integer
    int Random(int min, int max);

    /// @brief Generate random real
    /// @param min Min real value
    /// @param max Max real value
    /// @return Generated real
    double Random(double min, double max);
}

} // namespace rf
