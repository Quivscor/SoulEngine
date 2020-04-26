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
	static const uint MAX_BONES = 100;
private:
	std::vector<Texture> textures_loaded;
	std::string directory;
	std::vector<Mesh> meshes;

	std::map<std::string, uint> m_bone_mapping; // maps a bone name and their index
	uint m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif