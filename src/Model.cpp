﻿#include "Model.h"

Model::Model(std::vector<Mesh> meshes) : meshes(meshes)
{

}

Model::~Model()
{

}
Model::Model() 
{

}
void Model::setMeshes(std::vector<Mesh> meshez)
{
	meshes = meshez;
}
std::vector<Mesh> Model::GetMeshes()
{
	return meshes;
}
uint Model::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) {
			return i; }
	}

	assert(0);
	return 0;
}

uint Model::findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	
	for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) 
		{
			return i; 
		}
	}

	assert(0);
	return 0;
}

uint Model::findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	
	for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) 
		{
			return i; 
		}
	}

	assert(0);
	return 0;
}

aiVector3D Model::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1) 
	{
		return p_node_anim->mPositionKeys[0].mValue;
	}

	uint position_index = findPosition(p_animation_time, p_node_anim);
	uint next_position_index = position_index + 1; 
	assert(next_position_index < p_node_anim->mNumPositionKeys);

	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Model::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1) 
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	uint rotation_index = findRotation(p_animation_time, p_node_anim); 
	uint next_rotation_index = rotation_index + 1; 
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);
	
	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D Model::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1) 
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	uint scaling_index = findScaling(p_animation_time, p_node_anim);
	uint next_scaling_index = scaling_index + 1; 
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);

	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);

	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	for (uint i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i]; 
		if (std::string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}

	return nullptr;
}
// start from RootNode
void Model::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{

	std::string node_name(p_node->mName.data);

	
	const aiAnimation* animation = scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);
	if (node_anim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		
			node_transform = translate_matr * rotate_matr * scaling_matr;
		

	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;

	
	if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		uint bone_index = m_bone_mapping[node_name];
		m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
	}

	for (uint i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}

}

void Model::boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; // = mat4(1.0f);

	double time_in_ticks = time_in_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);
	readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

	transforms.resize(m_num_bones);

	for (uint i = 0; i < m_num_bones; i++)
	{
		transforms[i] = m_bone_matrices[i].final_world_transform;
	}
}

glm::mat4 Model::aiToGlm(aiMatrix4x4 ai_matr)
{
	glm::mat4 result;
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;



	return result;
}

aiQuaternion Model::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}
