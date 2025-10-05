#include "image.hpp"

#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <rf/core/error.hpp>

namespace rf {

Image::Image(const fs::path& filePath, bool verticalFlip) {
    load(filePath, verticalFlip);
}

Image::Image(const uint8_t* data, size_t length, bool verticalFlip) {
    load(data, length, verticalFlip);
}

Image::Image(Image&& other) noexcept
    : m_data(std::exchange(other.m_data, nullptr))
    , m_width(std::exchange(other.m_width, 0))
    , m_height(std::exchange(other.m_height, 0))
    , m_channels(std::exchange(other.m_channels, 0))
{}

Image::~Image() {
    free();
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        free();
        m_data = std::exchange(other.m_data, nullptr);
        m_width = std::exchange(other.m_width, 0);
        m_height = std::exchange(other.m_height, 0);
        m_channels = std::exchange(other.m_channels, 0);
    }
    return *this;
}

void Image::free() {
    if (m_data) {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

void Image::load(const fs::path& filePath, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load(filePath.string().c_str(), &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't read \"{}\" file to load image", filePath.string().c_str());
}

void Image::load(const uint8_t* data, size_t length, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load_from_memory(data, length, &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't read memory buffer to load image");
}

} // namespace rf
