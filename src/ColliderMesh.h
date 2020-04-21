#pragma once
#include "Core.h"
#include "Component.h"

struct ColliderVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
};

class ColliderMesh : public Component
{
public:
	ColliderMesh();
	ColliderMesh(std::vector<ColliderVertex> vertices, std::vector<unsigned int> indices);
	~ColliderMesh();

	std::vector<ColliderVertex> vertices;
	std::vector<ColliderVertex> originVertices;
	std::vector<unsigned int> indices;

	virtual ComponentType GetComponentType() const override { return ComponentType::ColliderMeshComponent; }

	unsigned int GetVAO() const;
	unsigned int GetVBO();
	unsigned int GetEBO();

	void setupMesh();

private:
	unsigned int VAO, VBO, EBO;
};