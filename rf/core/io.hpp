#pragma once

#include <string>

namespace rf {

namespace IO {
    std::string ReadFile(const std::string& filename);

    void WriteFile(const std::string& filename, const std::string& contents);
}

} // namespace rf
