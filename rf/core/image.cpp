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
    , m_dimensions(std::exchange(other.m_dimensions, {}))
{}

Image::~Image() {
    free();
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        free();
        m_data = std::exchange(other.m_data, nullptr);
        m_dimensions = std::exchange(other.m_dimensions, {});
    }
    return *this;
}

void Image::free() {
    if (m_data) {
        stbi_image_free(m_data);
        m_data = nullptr;
        m_dimensions = {};
    }
}

void Image::load(const fs::path& filePath, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load(filePath.string().c_str(), &m_dimensions.width, &m_dimensions.height, &m_dimensions.channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't read \"{}\" file to load image", filePath.string().c_str());
}

void Image::load(const uint8_t* data, size_t length, bool verticalFlip) {
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load_from_memory(data, length, &m_dimensions.width, &m_dimensions.height, &m_dimensions.channels, 4);
    if (!m_data)
        throw RF_LOCATED_ERROR("Couldn't read memory buffer to load image");
}

} // namespace rf
