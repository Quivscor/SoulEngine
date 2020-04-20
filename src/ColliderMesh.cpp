#include "ColliderMesh.h"

ColliderMesh::ColliderMesh()
{

}

ColliderMesh::ColliderMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices)
{
	this->originVertices = vertices;
	this->vertices = std::vector<glm::vec3>(vertices);
	this->indices = indices;
}

ColliderMesh::~ColliderMesh()
{

}