#include "rf/graphics/cubemap.hpp"

#include <map>
#include <filesystem>
#include <stdexcept>
namespace fs = std::filesystem;

#include "rf/engine/image.hpp"
#include "rf/engine/utility.hpp"

namespace rf {

Graphics::Cubemap::Cubemap(const std::string& cubemapDirectoryPath) {
    enum class Direction {
        Right,
        Left,
        Top,
        Bottom,
        Back,
        Front,
    };

    std::map<Direction, fs::path> files;
    for (const fs::directory_entry& file : fs::directory_iterator(cubemapDirectoryPath)) {
        std::string name = Engine::Utility::LowerCaseString(file.path().stem().string());
        if (name == "right")
            files[Direction::Right] = file.path();
        else if (name == "left")
            files[Direction::Left] = file.path();
        else if (name == "top")
            files[Direction::Top] = file.path();
        else if (name == "bottom")
            files[Direction::Bottom] = file.path();
        else if (name == "back")
            files[Direction::Back] = file.path();
        else if (name == "front")
            files[Direction::Front] = file.path();
    }
    if (files.size() != 6) {
        throw std::runtime_error(fmt::format(
            "rf::Graphics::Cubemap::Cubemap(): Couldn't load cubemap \"{}\": only {}/6 sides found",
            cubemapDirectoryPath, files.size()
        ));
    }

    try {
        glGenTextures(1, &m_cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);

        for (const auto& entry : files) {
            Engine::Image image(entry.second.string());
            unsigned int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(entry.first);
            glTexImage2D(target, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    catch (...) {
        glDeleteTextures(1, &m_cubemap);
        throw;
    }
}

Graphics::Cubemap::Cubemap(Cubemap&& other) noexcept
    : m_cubemap(other.m_cubemap) {
    other.m_cubemap = 0;
}

Graphics::Cubemap::~Cubemap() {
    if (m_cubemap) {
        glDeleteTextures(1, &m_cubemap);
    }
}

} // namespace rf
