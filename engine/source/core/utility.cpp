#include "rf/core/utility.hpp"

#include <chrono>
#include <random>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace rf {

spdlog::logger Utility::CreateLogger(const std::string& name) {
    static auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    static bool initialized = false;
    if (!initialized) {
        sink->set_pattern("[%^%d.%m.%C %H:%M:%S %L%$] [%n] %v");
        initialized = true;
    }
    return { name, {sink} };
}

void Utility::Sleep(double seconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<size_t>(seconds * 1'000'000)));
}

double Utility::Limit(double value, double min, double max) {
    if (value < min)
        value = min;
    if (value > max)
        value = max;
    return value;
}

int Utility::Random(int min, int max) {
    static std::mt19937 generator(std::random_device{}());
    return std::uniform_int_distribution(min, max)(generator);
}

double Utility::Random(double min, double max) {
    static std::mt19937 generator(std::random_device{}());
    return std::uniform_real_distribution(min, max)(generator);
}

void Utility::ToUpperCase(std::string& string) {
    for (char& character : string) {
        character = std::toupper(character);
    }
}

std::string Utility::UpperCaseString(std::string string) {
    ToUpperCase(string);
    return string;
}

void Utility::ToLowerCase(std::string& string) {
    for (char& character : string) {
        character = std::tolower(character);
    }
}

std::string Utility::LowerCaseString(std::string string) {
    ToLowerCase(string);
    return string;
}

} // namespace rf
