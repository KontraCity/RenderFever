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
    template <typename... Arguments>
    Error(const std::string& location, fmt::format_string<Arguments...> message, Arguments&&... arguments) noexcept {
        try {
            m_location = location;
            m_message = fmt::format(message, std::forward<Arguments>(arguments)...);
            withDetails("");
        }
        catch (...) {}
    }

public:
    Error& withDetails(const std::string& details) noexcept {
        try {
            m_details = details;
            if (m_details.empty())
                m_what = fmt::format("{}: {}", m_location, m_message);
            else
                m_what = fmt::format("{}: {} [{}]", m_location, m_message, m_details);
        }
        catch (...) {}
        return *this;
    }

    const std::string& location() const noexcept {
        return m_location;
    }

    const std::string& message() const noexcept {
        return m_message;
    }

    const std::string& details() const noexcept {
        return m_details;
    }

    const char* what() const noexcept override {
        return m_what.c_str();
    }
};

// rf::Error with throw location
#define \
    RF_LOCATED_ERROR(message, ...) \
    rf::Error(std::string(__FUNCTION__) + "()", message, ##__VA_ARGS__)

} // namespace rf
