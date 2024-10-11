#include "Model.h"

#include <iostream>

Model::Model()
{
}

Model::~Model()
{
}

void Model::loadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        std::cerr << "Model (" << fileName << ") failed to load: " << importer.GetErrorString() << std::endl;
    }

    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);
}

void Model::renderModel()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        const unsigned int materialIndex = meshToTex[i];

        if (materialIndex < textures.size() && textures[materialIndex])
        {
            textures[materialIndex]->useTexture();
        }

        meshes[i]->renderMesh();
    }
}

void Model::clearModel()
{
    for (const auto& mesh : meshes)
    {
        delete mesh;
    }
    meshes.clear();

    for (const auto& texture : textures)
    {
        delete texture;
    }
    textures.clear();
}

void Model::loadNode(const aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh(vertices.data(), indices.data(), vertices.size(), indices.size());
    meshes.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene* scene)
{
    textures.resize(scene->mNumMaterials);

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial* material = scene->mMaterials[i];
        textures[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                const int idx = std::string(path.data).rfind('\\');
                std::string fileName = std::string(path.data).substr(idx + 1);

                std::string texPath = "textures/" + fileName;

                textures[i] = new Texture(texPath.c_str());

                if (!textures[i]->loadTexture(true))
                {
                    std::cerr << "Failed to load texture at: " << texPath << std::endl;
                    delete textures[i];
                    textures[i] = nullptr;
                }
            }
        }

        if (!textures[i])
        {
            textures[i] = new Texture("textures/white.png");
            textures[i]->loadTexture(true);
        }
    }
}
