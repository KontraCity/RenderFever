#pragma once

#include <rf/auxiliary/fs.hpp>
#include <rf/auxiliary/gl.hpp>

#include <rf/graphics/storage.hpp>
#include <rf/resources/library.hpp>

namespace rf {

namespace Graphics {
    class Renderer {
    public:
        struct Config {
            fs::path mainShaderPath;
            fs::path lightShaderPath;
            size_t lightSourcesReserve = 1000;
            bool depthTestingMode = true;
            bool faceCullingMode = true;
            bool wireframeMode = false;
        };

        struct Shaders {
            Resources::Shader main;
            Resources::Shader light;
        };

    private:
        Shaders m_shaders;
        Storage m_lightStorage;
        bool m_depthTestingMode = true;
        bool m_faceCullingMode = true;
        bool m_wireframeMode = false;

    public:
        Renderer(const Config& config, const Resources::Library& library);

        Renderer(const Renderer& other) = delete;

        Renderer(Renderer&& other) noexcept;

        ~Renderer() = default;

    public:
        Renderer& operator=(const Renderer& other) = delete;

        Renderer& operator=(Renderer&& other) noexcept;

    private:
        void clear();

        void capture();

        void illuminate();

        void draw();

    public:
        void render();

    public:
        Shaders shaders() const {
            return m_shaders;
        }
        
        bool getDepthTestingMode() const {
            return m_depthTestingMode;
        }

        void setDepthTestingMode(bool depthTestingMode) {
            m_depthTestingMode = depthTestingMode;
            m_depthTestingMode ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        }

        bool getFaceCullingMode() const {
            return m_faceCullingMode;
        }

        void setFaceCullingMode(bool faceCullingMode) {
            m_faceCullingMode = faceCullingMode;
            m_faceCullingMode ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        }

        bool getWireframeMode() const {
            return m_wireframeMode;
        }

        void setWireframeMode(bool wireframeMode) {
            m_wireframeMode = wireframeMode;
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        }
    };
}

} // namespace rf
