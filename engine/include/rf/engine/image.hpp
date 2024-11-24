#pragma once

// STL modules
#include <stdexcept>

// Library {fmt}
#include <fmt/format.h>

// External modules
#include "rf/external/stb_image.h"

namespace rf {

namespace Engine
{
    class Image
    {
    private:
        uint8_t* m_data = nullptr;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;

    public:
        /// @brief Load image from file
        /// @param imageFilePath Path to the image file
        /// @param verticalFlip Whether to flip the image vertically on load or not
        /// @throw std::runtime_error if the image couldn't be loaded
        Image(const std::string& imageFilePath, bool verticalFlip = false);

        /// @brief Load image from buffer
        /// @param data The buffer to load
        /// @param length Length of the buffer
        /// @param verticalFlip Whether to flip the image vertically on load or not
        Image(const uint8_t* data, size_t length, bool verticalFlip = false);

        Image() = default;

        Image(const Image& other) = delete;

        Image(Image&& other) noexcept;

        ~Image();

    private:
        /// @brief Free allocated resources
        void free();

    public:
        /// @brief Load image from file
        /// @param imageFilePath Path to the image file
        /// @param verticalFlip Whether to flip the image vertically on load or not
        /// @throw std::runtime_error if the image couldn't be loaded
        void load(const std::string& imageFilePath, bool verticalFlip = false);

        /// @brief Load image from buffer
        /// @param data The buffer to load
        /// @param length Length of the buffer
        /// @param verticalFlip Whether to flip the image vertically on load or not
        void load(const uint8_t* data, size_t length, bool verticalFlip = false);

    public:
        /// @brief Check if image contains data
        /// @return True if image contains data
        inline operator bool() const
        {
            return static_cast<bool>(m_data);
        }

        /// @brief Get image data
        /// @return Image data
        inline const uint8_t* data() const
        {
            return m_data;
        }

        /// @brief Get image width
        /// @return Image width
        inline int width() const
        {
            return m_width;
        }

        /// @brief Get image height
        /// @return Image width
        inline int height() const
        {
            return m_height;
        }

        /// @brief Get number of image channels
        /// @return Number of image channels
        inline int channels() const
        {
            return m_channels;
        }
    };
}

} // namespace rf
