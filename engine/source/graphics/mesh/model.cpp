#include "rf/graphics/mesh/model.hpp"

// STL modules
#include <stdexcept>

// Library Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Library {fmt}
#include <fmt/format.h>

namespace rf {

Graphics::Mesh::Model::Model(const std::string& modelFilePath)
    : m_directory(fs::path(modelFilePath).parent_path())
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error(fmt::format("rf::Graphics::Model::Model(): Couldn't load model \"{}\"", modelFilePath));
    processNode(scene, scene->mRootNode);
}

void Graphics::Mesh::Model::processNode(const aiScene* scene, aiNode* node)
{
    m_meshes.reserve(m_meshes.size() + node->mNumMeshes);
    for (unsigned int index = 0; index < node->mNumMeshes; ++index)
        createMesh(scene, scene->mMeshes[node->mMeshes[index]]);
    for (unsigned int index = 0; index < node->mNumChildren; ++index)
        processNode(scene, node->mChildren[index]);
}

void Graphics::Mesh::Model::createMesh(const aiScene* scene, aiMesh* mesh)
{
    // Load vertices
    std::vector<Vertice> vertices(mesh->mNumVertices);
    for (unsigned int index = 0; index < mesh->mNumVertices; ++index)
    {
        Vertice& vertice = vertices[index];
        vertice.position = { mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z };
        vertice.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
        vertice.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y) : glm::vec2(0.0f);
    }

    // Load indices
    std::vector<Indice> indices;
    for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
    {
        indices.reserve(indices.size() + mesh->mNumFaces);
        for (unsigned int indiceIndex = 0; indiceIndex < mesh->mFaces[faceIndex].mNumIndices; ++indiceIndex)
            indices.push_back(mesh->mFaces[faceIndex].mIndices[indiceIndex]);
    }

    // Load textures
    Material material;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
        loadTextures(scene, meshMaterial, aiTextureType_DIFFUSE, material);
        loadTextures(scene, meshMaterial, aiTextureType_SPECULAR, material);
    }

    Mesh& createdMesh = m_meshes.emplace_back(vertices, indices);
    createdMesh.material() = material;
}

void Graphics::Mesh::Model::loadTextures(const aiScene* scene, aiMaterial* meshMaterial, aiTextureType type, Material& material)
{
    Texture::Pointer* targetTexture = nullptr;
    Texture::Type textureType = Texture::Type::None;
    switch (type)
    {
        case aiTextureType_DIFFUSE:
            targetTexture = &material.diffuse();
            textureType = Texture::Type::Diffuse;
            break;
        case aiTextureType_SPECULAR:
            targetTexture = &material.specular();
            textureType = Texture::Type::Specular;
            break;
        default:
            // Unknown texture! Can't load anything...
            return;
    }

    unsigned int textureCount = meshMaterial->GetTextureCount(type);
    for (unsigned int index = 0; index < textureCount; ++index)
    {
        aiString filename;
        meshMaterial->GetTexture(type, index, &filename);
        if (m_textures.find(filename.C_Str()) != m_textures.end())
        {
            *targetTexture = m_textures[filename.C_Str()];
            continue;
        }

        const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(filename.C_Str());
        if (embeddedTexture)
        {
            auto textureEntry = m_textures.try_emplace(filename.C_Str());
            textureEntry.first->second = std::make_shared<Texture>(reinterpret_cast<const uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth, textureType);
            *targetTexture = textureEntry.first->second;
        }
        else
        {
            auto textureEntry = m_textures.try_emplace(filename.C_Str());
            textureEntry.first->second = std::make_shared<Texture>((m_directory / fs::path(filename.C_Str())).string(), textureType);
            *targetTexture = textureEntry.first->second;
        }
    }
}

void Graphics::Mesh::Model::draw(Shader& shader) const
{
    m_transform.apply(shader);
    for (const Mesh& mesh : m_meshes)
        mesh.draw(shader);
}

} // namespace rf
