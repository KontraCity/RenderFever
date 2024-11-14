#include "utility.hpp"

namespace rf {

int Utility::Random(int min, int max)
{
    static std::mt19937 generator(std::random_device{}());
    return std::uniform_int_distribution(min, max)(generator);
}

double Utility::Random(double min, double max)
{
    static std::mt19937 generator(std::random_device{}());
    return std::uniform_real_distribution(min, max)(generator);
}

} // namespace rf
