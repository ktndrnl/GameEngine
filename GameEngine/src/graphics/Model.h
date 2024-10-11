﻿#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
    Model();
    ~Model();

    void loadModel(const std::string& fileName);
    void renderModel();
    void clearModel();
private:
    void loadNode(const aiNode* node, const aiScene* scene);
    void loadMesh(const aiMesh* mesh, const aiScene* scene);
    void loadMaterials(const aiScene* scene);

    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<unsigned int> meshToTex;
};
