#pragma once

#include <string>

#include <spdlog/spdlog.h>

namespace rf {

namespace Utility {
    spdlog::logger CreateLogger(const std::string& name);

    void Sleep(double seconds);

    double Limit(double value, double min, double max);

    int Random(int min, int max);

    double Random(double min, double max);

    void ToUpperCase(std::string& string);

    std::string UpperCaseString(std::string string);

    void ToLowerCase(std::string& string);

    std::string LowerCaseString(std::string string);
}

} // namespace rf
