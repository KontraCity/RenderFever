#pragma once

#include <vector>

#include <rf/core/zip.hpp>

namespace rf {

namespace Ui {
    template <auto& Array>
    const std::vector<uint8_t>& DecompressAndCache() {
        static std::vector<uint8_t> s_data;
        if (s_data.empty())
            s_data = Zip::Decompress(Array.data(), Array.size(), Zip::Mode::Gzip);
        return s_data;
    }
}

} // namespace rf
