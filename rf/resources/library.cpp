#include "library.hpp"

#include <regex>
#include <algorithm>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <fmt/format.h>

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

static void ProcessMesh(const aiScene* scene, aiMesh* mesh, std::vector<Graphics::Mesh::Vertice>& vertices, std::vector<Graphics::Mesh::Indice>& indices) {
    size_t offset = vertices.size();
    vertices.resize(offset + mesh->mNumVertices);
    for (unsigned int index = 0; index < mesh->mNumVertices; ++index) {
        Graphics::Mesh::Vertice& vertice = vertices[offset + index];
        vertice.position = { mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z };
        vertice.normal = { mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z };
        vertice.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y) : glm::vec2(0.0f);
    }

    for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
        offset = indices.size();
        indices.resize(offset + mesh->mFaces[faceIndex].mNumIndices);
        for (unsigned int indiceIndex = 0; indiceIndex < mesh->mFaces[faceIndex].mNumIndices; ++indiceIndex) {
            Graphics::Mesh::Indice& indice = indices[offset + indiceIndex];
            indice = mesh->mFaces[faceIndex].mIndices[indiceIndex];
        }
    }
}

static void ProcessNode(const aiScene* scene, aiNode* node, std::vector<Graphics::Mesh::Vertice>& vertices, std::vector<Graphics::Mesh::Indice>& indices) {
    for (unsigned int index = 0; index < node->mNumMeshes; ++index)
        ProcessMesh(scene, scene->mMeshes[node->mMeshes[index]], vertices, indices);
    for (unsigned int index = 0; index < node->mNumChildren; ++index)
        ProcessNode(scene, node->mChildren[index], vertices, indices);
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

Graphics::Texture Resources::Library::LoadTexture(const fs::path& path) {
    Image image(path);
    return { image };
}

Graphics::Mesh Resources::Library::LoadMesh(const fs::path& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw RF_LOCATED_ERROR("Couldn't read \"{}\" file to load model", path.string());

    std::vector<Graphics::Mesh::Vertice> vertices;
    std::vector<Graphics::Mesh::Indice> indices;
    ProcessNode(scene, scene->mRootNode, vertices, indices);
    return { vertices, indices };
}

Graphics::Mesh Resources::Library::LoadMesh(const std::vector<uint8_t>& data, const char* formatHint) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(data.data(), data.size(), aiProcess_Triangulate | aiProcess_FlipUVs, formatHint);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw RF_LOCATED_ERROR("Couldn't read memory buffer to load model");

    std::vector<Graphics::Mesh::Vertice> vertices;
    std::vector<Graphics::Mesh::Indice> indices;
    ProcessNode(scene, scene->mRootNode, vertices, indices);
    return { vertices, indices };
}

Resources::Library::Library(const fs::path& rootDirectory)
    : m_rootDirectory(rootDirectory)
{}

Resources::Shader Resources::Library::loadShader(fs::path path) {
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

Resources::Texture Resources::Library::loadTexture(fs::path path) {
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

Resources::Mesh Resources::Library::loadMesh(fs::path path) {
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

} // namespace rf
