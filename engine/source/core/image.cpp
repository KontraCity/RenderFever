#include "rf/core/image.hpp"

#include "rf/core/error.hpp"
#include "rf/external/stb_image.h"

namespace rf {

Image::Image(const std::string& imageFilePath, bool verticalFlip) {
    load(imageFilePath, verticalFlip);
}

Image::Image(const uint8_t* data, size_t length, bool verticalFlip) {
    load(data, length, verticalFlip);
}

Image::Image(Image&& other) noexcept
    : m_data(other.m_data)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_channels(other.m_channels) {
    other.m_data = nullptr;
    other.m_width = 0;
    other.m_height = 0;
    other.m_channels = 0;
}

Image::~Image() {
    free();
}

void Image::free() {
    if (m_data) {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

void Image::load(const std::string& imageFilePath, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load(imageFilePath.c_str(), &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't load image from \"{}\" file", imageFilePath);
}

void Image::load(const uint8_t* data, size_t length, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load_from_memory(data, length, &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't load image file from memory");
}

} // namespace rf
