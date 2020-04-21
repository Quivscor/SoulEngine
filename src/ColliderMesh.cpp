#include "ColliderMesh.h"

ColliderMesh::ColliderMesh()
{

}

ColliderMesh::ColliderMesh(std::vector<ColliderVertex> vertices, std::vector<unsigned int> indices)
{
	this->originVertices = vertices;
	this->vertices = std::vector<ColliderVertex>(vertices);
	this->indices = indices;

	setupMesh();
}

void ColliderMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, originVertices.size() * sizeof(ColliderVertex), &originVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColliderVertex), (void*)0);

	glBindVertexArray(0);
}


ColliderMesh::~ColliderMesh()
{

}

unsigned int ColliderMesh::GetVAO() const
{
	return VAO;
}

unsigned int ColliderMesh::GetVBO()
{
	return VBO;
}

unsigned int ColliderMesh::GetEBO()
{
	return EBO;
}