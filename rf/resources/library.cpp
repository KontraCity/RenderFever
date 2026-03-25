#include "library.hpp"

#include <regex>
#include <algorithm>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/core/error.hpp>
#include <rf/core/fio.hpp>
#include <rf/core/image.hpp>

namespace rf {

static fs::path NormalizePath(const fs::path& path, const fs::path& base = fs::current_path()) {
    fs::path absolute = fs::weakly_canonical(path);
    return fs::relative(absolute, base);
}

static fs::path GetFullPathAndNormalize(const fs::path& rootDirectory, fs::path& path) {
    fs::path fullPath = NormalizePath(rootDirectory / path);
    path = NormalizePath(fullPath, rootDirectory);
    return fullPath;
}

static fs::path GetFullPathAndNormalize(const fs::path& rootDirectory, const fs::path& path) {
    fs::path fullPath = NormalizePath(rootDirectory / path);
    return fullPath;
}

static std::string ReadShaderSource(fs::path path, std::vector<fs::path>& included) {
    path = fs::weakly_canonical(path);
    included.push_back(path);

    std::string source = FIO::ReadFile(path.string());
    while (true) {
        std::smatch matches;
        std::regex regex(R"(^[ \t]*#include[ \t]*["<](.+)[">][ \t]*$)");
        if (!std::regex_search(source, matches, regex))
            return source;

        std::string includeSource;
        fs::path includePath = fs::canonical(path.parent_path() / matches.str(1));
        if (std::find(included.begin(), included.end(), includePath) == included.end())
            includeSource = ReadShaderSource(path.parent_path() / matches.str(1), included);
        source.replace(matches.position(), matches.length(), includeSource);
    }
}

static std::string ReadShaderSource(const fs::path& path) {
    std::vector<fs::path> included;
    return ReadShaderSource(path, included);
}

static Resources::Texture ProcessModelTexture(const fs::path& path, const aiScene* scene, aiMaterial* material, aiTextureType textureType) {
    if (material->GetTextureCount(textureType) == 0)
        return {};

    aiString texturePath;
    material->GetTexture(textureType, 0, &texturePath);
    if (*texturePath.C_Str() != '*')
        // TODO: This is really easy to fix, maybe consider doing that?
        return {};

    const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str());
    return Engine::Library().loadTexture(
        path / texturePath.C_Str(),
        reinterpret_cast<const uint8_t*>(embeddedTexture->pcData),
        embeddedTexture->mWidth
    );
}

static void ProcessModelMesh(const fs::path& path, const aiScene* scene, aiMesh* mesh, std::vector<Graphics::Model::MatMesh>& meshes, std::vector<Graphics::Material>& materials) {
    std::vector<Graphics::Mesh::Vertice> vertices(mesh->mNumVertices);
    for (unsigned int index = 0; index < mesh->mNumVertices; ++index) {
        Graphics::Mesh::Vertice& vertice = vertices[index];
        vertice.position = { mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z };
        vertice.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
        vertice.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y) : glm::vec2(0.0f);
    }

    std::vector<Graphics::Mesh::Indice> indices;
    for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
        indices.reserve(indices.size() + mesh->mFaces[faceIndex].mNumIndices);
        for (unsigned int indiceIndex = 0; indiceIndex < mesh->mFaces[faceIndex].mNumIndices; ++indiceIndex)
            indices.push_back(mesh->mFaces[faceIndex].mIndices[indiceIndex]);
    }

    unsigned int matIndex = mesh->mMaterialIndex;
    if (matIndex + 1 > materials.size())
        materials.resize(matIndex + 1);

    meshes.push_back({
        .matIndex = static_cast<int>(matIndex),
        .mesh = { vertices, indices },
    });

    aiMaterial* material = scene->mMaterials[matIndex];
    materials[matIndex].diffuse = ProcessModelTexture(path, scene, material, aiTextureType_DIFFUSE);
    materials[matIndex].specular = ProcessModelTexture(path, scene, material, aiTextureType_SPECULAR);
}

static void ProcessModelNode(const fs::path& path, const aiScene* scene, aiNode* node, std::vector<Graphics::Model::MatMesh>& meshes, std::vector<Graphics::Material>& materials) {
    for (unsigned int index = 0; index < node->mNumMeshes; ++index)
        ProcessModelMesh(path, scene, scene->mMeshes[node->mMeshes[index]], meshes, materials);
    for (unsigned int index = 0; index < node->mNumChildren; ++index)
        ProcessModelNode(path, scene, node->mChildren[index], meshes, materials);
}

static Graphics::Model ProcessModelScene(const fs::path& path, const aiScene* scene) {
    std::vector<Graphics::Model::MatMesh> meshes;
    std::vector<Graphics::Material> materials;
    ProcessModelNode(path, scene, scene->mRootNode, meshes, materials);
    return { std::move(meshes), std::move(materials) };
}

static Graphics::Model ProcessModel(const std::vector<uint8_t>& data, const char* formatHint) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(data.data(), data.size(), aiProcess_Triangulate | aiProcess_FlipUVs, formatHint);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw RF_LOCATED_ERROR("Couldn't read memory buffer to process model");
    return ProcessModelScene("", scene);
}

static Graphics::Model ProcessModel(const fs::path& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw RF_LOCATED_ERROR("Couldn't read \"{}\" file to process model", path.string());
    return ProcessModelScene(path, scene);
}

static void LogError(const rf::Error& error) {
    fmt::print(stderr, "rf::Error! {}: {}\n", error.location(), error.message());
    if (!error.details().empty()) {
        fmt::print(stderr, "---- Details ----\n");
        fmt::print(stderr, "{}{}", error.details(), *(error.details().end() - 2) == '\n' ? "" : "\n");
        fmt::print(stderr, "-----------------\n");
    }
}

Graphics::Shader Resources::Library::LoadShader(const fs::path& path) {
    std::string name = path.stem().string();
    fs::path vertexFilePath = path / (name + ".vert");
    fs::path geometryFilePath = path / (name + ".geom");
    fs::path fragmentFilePath = path / (name + ".frag");

    Graphics::Shader::Config config = {
        .vertexSource = ReadShaderSource(vertexFilePath),
        .geometrySource = fs::is_regular_file(geometryFilePath) ? ReadShaderSource(geometryFilePath) : "",
        .fragmentSource = ReadShaderSource(fragmentFilePath),
    };
    return { config };
}

Graphics::Texture Resources::Library::LoadTexture(const uint8_t* data, size_t length) {
    Image image(data, length);
    return { image };
}

Graphics::Texture Resources::Library::LoadTexture(const fs::path& path) {
    Image image(path);
    return { image };
}

Graphics::Mesh Resources::Library::LoadMesh(const std::vector<uint8_t>& data, const char* formatHint) {
    Graphics::Model model = ProcessModel(data, formatHint);
    if (!model.isJustAMesh())
        throw RF_LOCATED_ERROR("Can't load mesh from a model that's not just a mesh");
    return model.extractJustAMesh();
}

Graphics::Mesh Resources::Library::LoadMesh(const fs::path& path) {
    Graphics::Model model = ProcessModel(path);
    if (!model.isJustAMesh())
        throw RF_LOCATED_ERROR("Can't load mesh from a model that's not just a mesh");
    return model.extractJustAMesh();
}

Graphics::Model Resources::Library::LoadModel(const std::vector<uint8_t>& data, const char* formatHint) {
    Graphics::Model model = ProcessModel(data, formatHint);
    if (model.isJustAMesh())
        throw RF_LOCATED_ERROR("Can't load model that's just a mesh");
    return model;
}

Graphics::Model Resources::Library::LoadModel(const fs::path& path) {
    Graphics::Model model = ProcessModel(path);
    if (model.isJustAMesh())
        throw RF_LOCATED_ERROR("Can't load model that's just a mesh");
    return model;
}

Resources::Library::Library(const fs::path& rootDirectory)
    : m_rootDirectory(rootDirectory)
{}

Resources::Shader Resources::Library::loadShader(const fs::path& path) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, path);
        Shader handle = m_shaders->get(path);
        if (!handle.isValid())
            handle = m_shaders->load(path, LoadShader(fullPath));
        return handle;
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
        return {};
    }
}

Resources::Texture Resources::Library::loadTexture(const fs::path& path, const uint8_t* data, size_t length) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, path);
        Texture handle = m_textures->get(path);
        if (!handle.isValid())
            handle = m_textures->load(path, LoadTexture(data, length));
        return handle;
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
        return {};
    }
}

Resources::Texture Resources::Library::loadTexture(const fs::path& path) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, path);
        Texture handle = m_textures->get(path);
        if (!handle.isValid())
            handle = m_textures->load(path, LoadTexture(fullPath));
        return handle;
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
        return {};
    }
}

Resources::Mesh Resources::Library::loadMesh(const fs::path& path) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, path);
        Mesh handle = m_meshes->get(path);
        if (!handle.isValid())
            handle = m_meshes->load(path, LoadMesh(fullPath));
        return handle;
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
        return {};
    }
}

Resources::Model Resources::Library::loadModel(const fs::path& path) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, path);
        Model handle = m_models->get(path);
        if (!handle.isValid())
            handle = m_models->load(path, LoadModel(fullPath));
        return handle;
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
        return {};
    }
}

void Resources::Library::reloadShader(const Shader& shader) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, shader.path());
        m_shaders->reload(shader, LoadShader(fullPath));
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
    }
}

void Resources::Library::reloadTexture(const Texture& texture) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, texture.path());
        m_textures->reload(texture, LoadTexture(fullPath));
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
    }
}

void Resources::Library::reloadMesh(const Mesh& mesh) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, mesh.path());
        m_meshes->reload(mesh, LoadMesh(fullPath));
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
    }
}

void Resources::Library::reloadModel(const Model& model) {
    try {
        fs::path fullPath = GetFullPathAndNormalize(m_rootDirectory, model.path());
        m_models->reload(model, LoadModel(fullPath));
    }
    catch (const Error& error) {
        // TODO: Log error with spdlog
        LogError(error);
    }
}

} // namespace rf
