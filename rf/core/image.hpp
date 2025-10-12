#pragma once

#include <cstdint>

#include <rf/auxiliary/fs.hpp>

namespace rf {

class Image {
public:
    struct Dimensions {
        int width = 0;
        int height = 0;
        int channels = 0;
    };

private:
    uint8_t* m_data = nullptr;
    Dimensions m_dimensions = {};

public:
    Image(const fs::path& filePath, bool verticalFlip = false);

    Image(const uint8_t* data, size_t length, bool verticalFlip = false);

    Image() = default;

    Image(const Image& other) = delete;

    Image(Image&& other) noexcept;

    ~Image();

public:
    Image& operator=(const Image& other) = delete;

    Image& operator=(Image&& other) noexcept;

private:
    void free();

public:
    void load(const fs::path& filePath, bool verticalFlip = false);

    void load(const uint8_t* data, size_t length, bool verticalFlip = false);

public:
    const uint8_t* data() const {
        return m_data;
    }

    const Dimensions& dimensions() const {
        return m_dimensions;
    }

public:
    operator bool() const {
        return m_data != nullptr;
    }
};

} // namespace rf
