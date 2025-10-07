#include "zip.hpp"

#include <utility>
#include <algorithm>

#include <zlib.h>

#include <rf/core/error.hpp>

namespace rf {

constexpr int ZipModeToWindowBits(Zip::Mode mode) {
    switch (mode) {
        case Zip::Mode::Gzip:   return 16 + MAX_WBITS;
        case Zip::Mode::Zlib:   return MAX_WBITS;
        case Zip::Mode::Raw:    return -MAX_WBITS;
    }
    throw RF_LOCATED_ERROR("Unknown zip mode: {}", std::to_underlying(mode));
}

constexpr int ZipLevelToInteger(Zip::Level level) {
    switch (level) {
        case Zip::Level::NoCompression:     return Z_NO_COMPRESSION;
        case Zip::Level::BestSpeed:         return Z_BEST_SPEED;
        case Zip::Level::Default:           return Z_DEFAULT_COMPRESSION;
        case Zip::Level::BestCompression:   return Z_BEST_COMPRESSION;
    }
    throw RF_LOCATED_ERROR("Unknown zip level: {}", std::to_underlying(level));
}

std::vector<uint8_t> Zip::Compress(const uint8_t* data, size_t length, Mode mode, Level level) {
    if (length == 0)
        return {};
    std::vector<uint8_t> compressed(std::max<size_t>(length / 2, 256));

    z_stream stream = {};
    stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data));
    stream.avail_in = static_cast<uInt>(length);
    stream.next_out = reinterpret_cast<Bytef*>(compressed.data());
    stream.avail_out = static_cast<uInt>(compressed.size());

    int result = deflateInit2(&stream, ZipLevelToInteger(level), Z_DEFLATED, ZipModeToWindowBits(mode), 8, Z_DEFAULT_STRATEGY);
    if (result != Z_OK)
        throw RF_LOCATED_ERROR("Couldn't initialize zlib deflate: result is {}", result);

    while (true) {
        result = deflate(&stream, stream.avail_in ? Z_NO_FLUSH : Z_FINISH);
        if (result == Z_STREAM_END)
            break;

        if (result == Z_BUF_ERROR) {
            size_t used = compressed.size() - stream.avail_out;
            compressed.resize(compressed.size() * 2);
            stream.next_out = reinterpret_cast<Bytef*>(compressed.data() + used);
            stream.avail_out = static_cast<uInt>(compressed.size() - used);
        }
        else if (result != Z_OK) {
            deflateEnd(&stream);
            throw RF_LOCATED_ERROR("Couldn't deflate: result is {}", result);
        }
    }

    compressed.resize(stream.total_out);
    deflateEnd(&stream);
    return compressed;
}

std::vector<uint8_t> Zip::Decompress(const uint8_t* data, size_t length, Mode mode) {
    if (length == 0)
        return {};
    std::vector<uint8_t> decompressed(std::max<size_t>(length * 2, 256));

    z_stream stream = {};
    stream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data));
    stream.avail_in = static_cast<uInt>(length);
    stream.next_out = reinterpret_cast<Bytef*>(decompressed.data());
    stream.avail_out = static_cast<uInt>(decompressed.size());

    int result = inflateInit2(&stream, ZipModeToWindowBits(mode));
    if (result != Z_OK)
        throw RF_LOCATED_ERROR("Couldn't initialize zlib inflate: result is {}", result);

    while (true) {
        result = inflate(&stream, Z_NO_FLUSH);
        if (result == Z_STREAM_END)
            break;

        if (result == Z_BUF_ERROR) {
            size_t used = decompressed.size() - stream.avail_out;
            decompressed.resize(decompressed.size() * 2);
            stream.next_out = reinterpret_cast<Bytef*>(decompressed.data() + used);
            stream.avail_out = static_cast<uInt>(decompressed.size() - used);
        }
        else if (result != Z_OK) {
            inflateEnd(&stream);
            throw RF_LOCATED_ERROR("Couldn't inflate: result is {}", result);
        }
    }

    decompressed.resize(stream.total_out);
    inflateEnd(&stream);
    return decompressed;
}

} // namespace rf
