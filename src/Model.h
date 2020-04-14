#pragma once
#include "Core.h"
#include "Mesh.h"

class Model
{
public:
	Model(std::vector<Mesh> meshes);
	~Model();

	std::vector<Mesh> GetMeshes();

private:
	std::vector<Mesh> meshes;
};