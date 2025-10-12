#pragma once

#include <rf/auxiliary/fs.hpp>
#include <rf/auxiliary/gl.hpp>

#include <rf/graphics/framebuffer.hpp>
#include <rf/graphics/storage.hpp>
#include <rf/resources/library.hpp>
#include <rf/world/scene.hpp>

namespace rf {

namespace Graphics {
    class Renderer {
    public:
        struct Config {
            fs::path mainShaderPath;
            fs::path lightShaderPath;
            fs::path neutralShaderPath;
            size_t lightSourcesReserve = 100;
            Dimensions previewFramebufferDimensions = { 500, 500 };

            bool depthTestingMode = true;
            bool faceCullingMode = true;
            bool wireframeMode = false;
        };

    private:
        Resources::Shader m_mainShader;
        Resources::Shader m_lightShader;
        Storage m_lightStorage;
        Framebuffer m_previewFramebuffer;

        bool m_depthTestingMode = true;
        bool m_faceCullingMode = true;
        bool m_wireframeMode = false;

    public:
        Renderer(const Config& config, Resources::Library& library);

        Renderer(const Renderer& other) = delete;

        Renderer(Renderer&& other) noexcept;

        ~Renderer() = default;

    public:
        Renderer& operator=(const Renderer& other) = delete;

        Renderer& operator=(Renderer&& other) noexcept;

    private:
        void clear();

        void capture(const World::Scene& scene);

        void illuminate(const World::Scene& scene);

        void draw(const World::Scene& scene);

    public:
        // Passing preview scene will force the renderer to render to preview framebuffer
        void render(const World::Scene* previewScene = nullptr);

    public:
        const Framebuffer& previewFramebuffer() const {
            return m_previewFramebuffer;
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
