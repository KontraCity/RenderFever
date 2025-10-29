#pragma once

#include <memory>
#include <unordered_map>

#include <rf/graphics/texture.hpp>
#include <rf/resources/library.hpp>

namespace rf {

namespace Ui {
    class PreviewMap {
    public:
        using ShaderPreviews = std::unordered_map<Resources::ShaderId, Graphics::Texture>;
        using MeshPreviews = std::unordered_map<Resources::MeshId, Graphics::Texture>;

    private:
        ShaderPreviews m_shaderPreviews;
        MeshPreviews m_meshPreviews;

        // Container fields for shader preview
        std::shared_ptr<Graphics::Mesh> m_cubeMesh;
        std::shared_ptr<Graphics::Texture> m_containerDiffuseTexture;
        std::shared_ptr<Graphics::Texture> m_containerSpecularTexture;

    public:
        PreviewMap();

        PreviewMap(const PreviewMap& other) = delete;

        PreviewMap(PreviewMap&& other) noexcept = default;

        ~PreviewMap() = default;

    public:
        PreviewMap& operator=(const PreviewMap& other) = delete;

        PreviewMap& operator=(PreviewMap&& other) noexcept = default;

    private:
        Graphics::Texture renderShaderPreview(const Resources::Shader& shader);

        Graphics::Texture renderMeshPreview(const Resources::Mesh& mesh);

    public:
        bool containsShaderPreview(const Resources::Shader& shader) const;

        bool containsMeshPreview(const Resources::Mesh& mesh) const;

        const Graphics::Texture& getShaderPreview(const Resources::Shader& shader);

        const Graphics::Texture& getMeshPreview(const Resources::Mesh& mesh);

        void resetShaderPreview(const Resources::Shader& shader);

        void resetMeshPreview(const Resources::Mesh& mesh);
    };
}

} // namespace rf
