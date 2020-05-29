#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Core.h"
#include "Transform.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Model.h"
#include "stb_image.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();
	Model* LoadModel(std::string path);

private:
	std::vector<Texture> textures_loaded;
	std::string directory;
	std::vector<Mesh> meshes;

	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	void ProcessNode(aiNode *node, const aiScene *scene, Model* model);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, Model* model);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif