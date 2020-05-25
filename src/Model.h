#pragma once
#include "Core.h"
#include "Mesh.h"

class Mesh;
struct VertexBoneData;
class Model : public Component
{
public:
	Model(std::vector<Mesh> meshes);
	Model();

	~Model();
	static const uint MAX_BONES = 100;
	std::vector<Mesh> GetMeshes();
	Assimp::Importer import;
	const aiScene* scene;
	aiScene** scenes;
	std::map<std::string, uint> m_bone_mapping; // maps a bone name and their index
	uint m_num_bones = 0;	
	std::vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;
	void setMeshes(std::vector<Mesh> meshes);
	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;
	void Model::ChangeBonePositions();
	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend); // super super n lerp =)
	void initShaders(Shader *shader_program);
	void UseModel(Model* model);

	std::vector<Mesh> meshes;
	double time = 0;
private:
	
	uint findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name);
	// calculate transform matrix
	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
	void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);

};