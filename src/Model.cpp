#include "Model.h"

Model::Model(std::vector<Mesh> meshes) : meshes(meshes)
{

}

Model::~Model()
{

}

std::vector<Mesh> Model::GetMeshes()
{
	return meshes;
}