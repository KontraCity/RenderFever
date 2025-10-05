#pragma once

#include <cstdint>

#include <rf/auxiliary/fs.hpp>

namespace rf {

class Image {
private:
    uint8_t* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

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

    int width() const {
        return m_width;
    }

    int height() const {
        return m_height;
    }

    int channels() const {
        return m_channels;
    }

public:
    operator bool() const {
        return m_data != nullptr;
    }
};

} // namespace rf
