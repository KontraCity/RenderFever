#include "library.hpp"

#include <vector>
#include <regex>
#include <algorithm>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <rf/core/error.hpp>
#include <rf/core/fio.hpp>
#include <rf/core/image.hpp>

namespace rf {

static fs::path NormalizePath(const fs::path& path) {
    fs::path absolute = fs::weakly_canonical(path);
    fs::path base = fs::current_path();
    return fs::relative(absolute, base);
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

static Graphics::Mesh LoadMesh(const fs::path& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw RF_LOCATED_ERROR("Couldn't read \"{}\" file to load model", path.string());

    std::vector<Graphics::Mesh::Vertice> vertices;
    std::vector<Graphics::Mesh::Indice> indices;
    ProcessNode(scene, scene->mRootNode, vertices, indices);
    return { vertices, indices };
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

Resources::Library::Library(const Config& config)
    : m_meshesBasePath(config.resourcesPath / config.meshesPath)
    , m_shadersBasePath(config.resourcesPath / config.shadersPath)
    , m_texturesBasePath(config.resourcesPath / config.texturesPath)
{}

Resources::Mesh Resources::Library::loadMesh(fs::path path) {
    path = NormalizePath(m_meshesBasePath / path);

    Mesh handle = m_meshes->get(path);
    if (!handle.isValid())
        handle = m_meshes->load(path, {}, LoadMesh(path));
    return handle;
}

Resources::Shader Resources::Library::loadShader(fs::path path) {
    path = NormalizePath(m_shadersBasePath / path);

    Shader handle = m_shaders->get(path);
    if (!handle.isValid()) {
        std::string name = path.stem().string();
        ShaderInfo info = {
            .vertexFilePath = path / (name + ".vert"),
            .geometryFilePath = path / (name + ".geom"),
            .fragmentFilePath = path / (name + ".frag"),
            .geometryStage = fs::is_regular_file(path / (name + ".geom"))
        };
        handle = m_shaders->load(path, info, Graphics::Shader::Config{
            .vertexSource = ReadShaderSource(info.vertexFilePath),
            .geometrySource = info.geometryStage ? ReadShaderSource(info.geometryFilePath) : "",
            .fragmentSource = ReadShaderSource(info.fragmentFilePath),
        });
    }
    return handle;
}

Resources::Texture Resources::Library::loadTexture(fs::path path) {
    path = NormalizePath(m_texturesBasePath / path);

    Texture handle = m_textures->get(path);
    if (!handle.isValid()) {
        Image image(fs::weakly_canonical(path));
        TextureInfo info = { image.dimensions() };
        handle = m_textures->load(path, info, { image });
    }
    return handle;
}

} // namespace rf
