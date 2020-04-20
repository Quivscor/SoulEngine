#pragma once
#include "Core.h"
#include "Component.h"

class ColliderMesh : public Component
{
public:
	ColliderMesh();
	ColliderMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
	~ColliderMesh();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> originVertices;
	std::vector<unsigned int> indices;

	virtual ComponentType GetComponentType() const override { return ComponentType::ColliderMeshComponent; }

private:

};