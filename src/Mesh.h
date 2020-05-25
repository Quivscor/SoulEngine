#pragma once
#include "Core.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"

typedef unsigned int uint;
#define NUM_BONES_PER_VEREX 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct BoneMatrix
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;

};
struct VertexBoneData
{
	uint ids[NUM_BONES_PER_VEREX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
	float weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));    // init all values in array = 0
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(uint bone_id, float weight);
};
class Mesh : public Component
{
public:
	Mesh(std::vector<Vertex> vertic, std::vector<GLuint> ind, std::vector<Texture> textur, std::vector<VertexBoneData> bone_id_weights);

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();
	std::vector<VertexBoneData> bones_id_weights_for_each_vertex;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;
	void SetAll(Mesh m);
	unsigned int GetVAO() const;
	unsigned int GetVBO();
	unsigned int GetEBO();
	void Draw(Shader* shaders_program);
	virtual ComponentType GetComponentType() const override { return ComponentType::MeshComponent; }

	void setupMesh();
	void setupMeshfBones();
	void SetDrawRadius(float radius);
	float GetDrawRadius();
private:
	unsigned int VAO, VBO, EBO;
	GLuint VBO_vertices;
	GLuint VBO_bones;

	float drawRadius = 0;
};