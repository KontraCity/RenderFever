#pragma once

#include <rf/graphics/texture.hpp>
#include <rf/ui/embedded/flaticon_icons_png.hpp>

namespace rf {

namespace Ui {
    class IconMap {
    private:
        Graphics::Texture m_regularDirectory;
        Graphics::Texture m_tinyDirectory;
        Graphics::Texture m_regularFile;
        Graphics::Texture m_tinyFile;
        Graphics::Texture m_regularGear;
        Graphics::Texture m_tinyGear;
        Graphics::Texture m_regularImage;
        Graphics::Texture m_tinyImage;
        Graphics::Texture m_regularCube;
        Graphics::Texture m_tinyCube;
        Graphics::Texture m_regularQuestion;
        Graphics::Texture m_tinyQuestion;
        Graphics::Texture m_tinyLeft;
        Graphics::Texture m_tinyRight;
        Graphics::Texture m_tinyUp;
        Graphics::Texture m_regularFileVert;
        Graphics::Texture m_regularFileGeom;
        Graphics::Texture m_regularFileFrag;

    public:
        IconMap()
            : m_regularDirectory(Image(Embedded::DirectoryRegularIconPng()))
            , m_tinyDirectory(Image(Embedded::DirectoryTinyIconPng()))
            , m_regularFile(Image(Embedded::FileRegularIconPng()))
            , m_tinyFile(Image(Embedded::FileTinyIconPng()))
            , m_regularGear(Image(Embedded::GearRegularIconPng()))
            , m_tinyGear(Image(Embedded::GearTinyIconPng()))
            , m_regularImage(Image(Embedded::ImageRegularIconPng()))
            , m_tinyImage(Image(Embedded::ImageTinyIconPng()))
            , m_regularCube(Image(Embedded::CubeRegularIconPng()))
            , m_tinyCube(Image(Embedded::CubeTinyIconPng()))
            , m_regularQuestion(Image(Embedded::QuestionRegularIconPng()))
            , m_tinyQuestion(Image(Embedded::QuestionTinyIconPng()))
            , m_tinyLeft(Image(Embedded::LeftTinyIconPng()))
            , m_tinyRight(Image(Embedded::RightTinyIconPng()))
            , m_tinyUp(Image(Embedded::UpTinyIconPng()))
            , m_regularFileVert(Image(Embedded::FileVertRegularIconPng()))
            , m_regularFileGeom(Image(Embedded::FileGeomRegularIconPng()))
            , m_regularFileFrag(Image(Embedded::FileFragRegularIconPng()))
        {}

        IconMap(const IconMap& other) = delete;

        IconMap(IconMap&& other) noexcept = default;

        ~IconMap() = default;

    public:
        IconMap& operator=(const IconMap& other) = delete;

        IconMap& operator=(IconMap&& other) noexcept = default;

    public:
        const Graphics::Texture& regularDirectory() const {
            return m_regularDirectory;
        }

        const Graphics::Texture& tinyDirectory() const {
            return m_tinyDirectory;
        }

        const Graphics::Texture& regularFile() const {
            return m_regularFile;
        }

        const Graphics::Texture& tinyFile() const {
            return m_tinyFile;
        }

        const Graphics::Texture& regularGear() const {
            return m_regularGear;
        }

        const Graphics::Texture& tinyGear() const {
            return m_tinyGear;
        }

        const Graphics::Texture& regularImage() const {
            return m_regularImage;
        }

        const Graphics::Texture& tinyImage() const {
            return m_tinyImage;
        }

        const Graphics::Texture& regularCube() const {
            return m_regularCube;
        }

        const Graphics::Texture& tinyCube() const {
            return m_tinyCube;
        }

        const Graphics::Texture& regularQuestion() const {
            return m_regularQuestion;
        }

        const Graphics::Texture& tinyQuestion() const {
            return m_tinyQuestion;
        }

        const Graphics::Texture& tinyLeft() const {
            return m_tinyLeft;
        }

        const Graphics::Texture& tinyRight() const {
            return m_tinyRight;
        }

        const Graphics::Texture& tinyUp() const {
            return m_tinyUp;
        }

        const Graphics::Texture& regularFileVert() const {
            return m_regularFileVert;
        }

        const Graphics::Texture& regularFileGeom() const {
            return m_regularFileGeom;
        }

        const Graphics::Texture& regularFileFrag() const {
            return m_regularFileFrag;
        }
    };
}

} // namespace rf
