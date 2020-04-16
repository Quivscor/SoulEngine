#pragma once
#include "Core.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh : public Component
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;

	unsigned int GetVAO() const;
	unsigned int GetVBO();
	unsigned int GetEBO();

	virtual ComponentType GetComponentType() const override { return ComponentType::MeshComponent; }

	void setupMesh();

private:
	unsigned int VAO, VBO, EBO;
};