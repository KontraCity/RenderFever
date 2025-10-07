#pragma once

#include <cstdint>
#include <vector>

namespace rf {

namespace Zip {
    enum class Mode {
        Gzip,   // DEFLATE with gzip header and footer (RFC 1952)
        Zlib,   // DEFLATE with zlib header and Andler32 checksum (RFC 1950)
        Raw,    // Raw DEFLATE stream without any wrapper
    };

    enum class Level {
        NoCompression,      // Level 0 - no compression
        BestSpeed,          // Level 1 - quickest compression
        Default,            // Level 6 - balanced speed/ratio compression
        BestCompression,    // Level 9 - best compression
    };

    std::vector<uint8_t> Compress(const uint8_t* data, size_t length, Mode mode, Level level = Level::Default);

    std::vector<uint8_t> Decompress(const uint8_t* data, size_t length, Mode mode);
}

} // namespace rf
