#include "rf/engine/image.hpp"

// STL modules
#include <stdexcept>

// Library {fmt}
#include <fmt/format.h>

// External modules
#include "rf/external/stb_image.h"

namespace rf {

Engine::Image::Image(const std::string& imageFilePath, bool verticalFlip)
{
    load(imageFilePath, verticalFlip);
}

Engine::Image::Image(const uint8_t* data, size_t length, bool verticalFlip)
{
    load(data, length, verticalFlip);
}

Engine::Image::Image(Image&& other) noexcept
    : m_data(other.m_data)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_channels(other.m_channels)
{
    other.m_data = nullptr;
    other.m_width = 0;
    other.m_height = 0;
    other.m_channels = 0;
}

Engine::Image::~Image()
{
    free();
}

void Engine::Image::free()
{
    if (m_data)
    {
        stbi_image_free(m_data);
        m_data = nullptr;
    }
}

void Engine::Image::load(const std::string& imageFilePath, bool verticalFlip)
{
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load(imageFilePath.c_str(), &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw std::runtime_error(fmt::format("rf::Engine::Image::load(): Couldn't load image file \"{}\"", imageFilePath));
}

void Engine::Image::load(const uint8_t* data, size_t length, bool verticalFlip)
{
    free(); // avoid memory leaks if load() was called already
    stbi_set_flip_vertically_on_load(verticalFlip);
    m_data = stbi_load_from_memory(data, length, &m_width, &m_height, &m_channels, 4);
    if (!m_data)
        throw std::runtime_error("rf::Engine::Image::load(): Couldn't load image file");
}

} // namespace rf
