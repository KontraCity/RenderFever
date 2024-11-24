#include "rf/graphics/model.hpp"

namespace rf {

void Graphics::Model::processNode(const aiScene* scene, aiNode* node)
{
    m_meshes.reserve(m_meshes.size() + node->mNumMeshes);
    for (unsigned int index = 0; index < node->mNumMeshes; ++index)
        createMesh(scene, scene->mMeshes[node->mMeshes[index]]);
    for (unsigned int index = 0; index < node->mNumChildren; ++index)
        processNode(scene, node->mChildren[index]);
}

void Graphics::Model::createMesh(const aiScene* scene, aiMesh* mesh)
{
    // Load vertices
    std::vector<Mesh::Vertice> vertices(mesh->mNumVertices);
    for (unsigned int index = 0; index < mesh->mNumVertices; ++index)
    {
        Mesh::Vertice& vertice = vertices[index];
        vertice.position = { mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z };
        vertice.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
        vertice.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y) : glm::vec2(0.0f);
    }

    // Load indices
    std::vector<Mesh::Indice> indices;
    for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
    {
        indices.reserve(indices.size() + mesh->mNumFaces);
        for (unsigned int indiceIndex = 0; indiceIndex < mesh->mFaces[faceIndex].mNumIndices; ++indiceIndex)
            indices.push_back(mesh->mFaces[faceIndex].mIndices[indiceIndex]);
    }

    // Load textures
    std::vector<Texture::Pointer> textures;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadTextures(scene, material, aiTextureType_DIFFUSE, textures);
        loadTextures(scene, material, aiTextureType_SPECULAR, textures);
    }

    m_meshes.emplace_back(vertices, indices, textures);
}

void Graphics::Model::loadTextures(const aiScene* scene, aiMaterial* material, aiTextureType type, std::vector<Texture::Pointer>& textures)
{
    Texture::Type textureType = Texture::Type::None;
    switch (type)
    {
        case aiTextureType_DIFFUSE:
            textureType = Texture::Type::Diffuse;
            break;
        case aiTextureType_SPECULAR:
            textureType = Texture::Type::Specular;
            break;
        default:
            // Unknown texture! Can't load anything...
            return;
    }

    unsigned int textureCount = material->GetTextureCount(type);
    textures.reserve(textures.size() + textureCount);
    for (unsigned int index = 0; index < textureCount; ++index)
    {
        aiString filename;
        material->GetTexture(type, index, &filename);

        if (m_textures.find(filename.C_Str()) != m_textures.end())
        {
            textures.push_back(m_textures[filename.C_Str()]);
            continue;
        }

        const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(filename.C_Str());
        if (embeddedTexture)
        {
            auto textureEntry = m_textures.try_emplace(filename.C_Str());
            textureEntry.first->second = std::make_shared<Texture>(reinterpret_cast<const uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth, textureType);
            textures.push_back(textureEntry.first->second);
        }
        else
        {
            auto textureEntry = m_textures.try_emplace(filename.C_Str());
            textureEntry.first->second = std::make_shared<Texture>((m_directory / fs::path(filename.C_Str())).string(), textureType);
            textures.push_back(textureEntry.first->second);
        }
    }
}

void Graphics::Model::load(const std::string& modelFilePath)
{
    m_directory = fs::path(modelFilePath).parent_path();
    m_meshes.clear();
    m_textures.clear();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw std::runtime_error(fmt::format("rf::Graphics::Model::load(): Couldn't load model \"{}\"", modelFilePath));
    processNode(scene, scene->mRootNode);
}

void Graphics::Model::draw(Shader& shader) const
{
    // Apply
    m_transform.apply(shader);

    // Draw
    shader.set("Material.shininess", 32.0f);
    for (const Mesh& mesh : m_meshes)
        mesh.draw(shader);
}

} // namespace rf
