#pragma once

#include <cstdint>
#include <string>

namespace rf {

class Image {
private:
    uint8_t* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

public:
    Image(const std::string& imageFilePath, bool verticalFlip = false);

    Image(const uint8_t* data, size_t length, bool verticalFlip = false);

    Image() = default;

    Image(const Image& other) = delete;

    Image(Image&& other) noexcept;

    ~Image();

private:
    void free();

public:
    void load(const std::string& imageFilePath, bool verticalFlip = false);

    void load(const uint8_t* data, size_t length, bool verticalFlip = false);

public:
    inline operator bool() const {
        return static_cast<bool>(m_data);
    }

    inline const uint8_t* data() const {
        return m_data;
    }

    inline int width() const {
        return m_width;
    }

    inline int height() const {
        return m_height;
    }

    inline int channels() const {
        return m_channels;
    }
};

} // namespace rf
