#pragma once

// STL modules
#include <chrono>
#include <random>
#include <string>

// Library spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace rf {

namespace Engine
{
    namespace Utility
    {
        /// @brief Create logger
        /// @param name Logger name
        /// @return Created logger
        spdlog::logger CreateLogger(const std::string& name);

        /// @brief Perform execution delay
        /// @param seconds Amount of seconds to delay for
        void Sleep(double seconds);

        /// @brief Limit value to an inclusive range
        /// @param value The value to limit
        /// @param min Min result value
        /// @param max Max result value
        /// @return Limited value
        double Limit(double value, double min, double max);

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

        /// @brief Convert string to upper case
        /// @param string The string to convert
        void ToUpperCase(std::string& string);

        /// @brief Get upper case version of a string
        /// @param string The string to get upper case version of
        /// @return Upper case version of the string
        std::string UpperCaseString(std::string string);

        /// @brief Convert string to lower case
        /// @param string The string to convert
        void ToLowerCase(std::string& string);

        /// @brief Get lower case version of a string
        /// @param string The string to get lower case version of
        /// @return Lower case version of the string
        std::string LowerCaseString(std::string string);
    }
}

} // namespace rf
