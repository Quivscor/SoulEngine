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

class Mesh : Component
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;

	unsigned int GetVAO() const;
	unsigned int GetVBO();
	unsigned int GetEBO();

private:
	void setupMesh();

	unsigned int VAO, VBO, EBO;
};