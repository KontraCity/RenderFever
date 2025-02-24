#pragma once

#include <string>
#include <stdexcept>

#include <fmt/format.h>

namespace rf {

class Error : public std::exception {
private:
    std::string m_location;
    std::string m_message;
    std::string m_details;
    std::string m_what;

public:
    template <typename... Parameters>
    Error(const std::string& location, const std::string& details, fmt::format_string<Parameters...> message, Parameters&&... parameters)
        : m_location(location)
        , m_message(fmt::format(message, std::forward<Parameters>(parameters)...))
        , m_details(details) {
        if (m_details.empty())
            m_what = fmt::format("{}: {}", m_location, m_message);
        else
            m_what = fmt::format("{}: {} [{}]", m_location, m_message, m_details);
    }

public:
    inline const std::string& location() const {
        return m_location;
    }

    inline const std::string& message() const {
        return m_message;
    }

    inline const std::string& details() const {
        return m_details;
    }

    inline const char* what() const noexcept override {
        return m_what.c_str();
    }
};

// Error that includes current function name as location
#define \
    RF_LOCATED_ERROR(message, ...) \
    Error(std::string(__FUNCTION__) + "()", "", message, __VA_ARGS__);

// Error that includes current function name as location and error details
#define \
    RF_LOCATED_DETAILED_ERROR(details, message, ...) \
    Error(std::string(__FUNCTION__) + "()", details, message, __VA_ARGS__)

} // namespace rf
