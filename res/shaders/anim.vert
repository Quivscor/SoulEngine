#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 bone_ids;     // INT pointer
layout(location = 4) in vec4 weights;



out vec3 Color;
out vec2 TexCoords;
out vec3 normal;

uniform mat4 M_matrix;
uniform mat4 normals_matrix;
uniform mat4 transform;
uniform vec3 color;
const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
	mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
			
	vec4 boned_position = bone_transform * vec4(aPos, 1.0); // transformed by bones

	normal = normalize(vec3(normals_matrix * (bone_transform * vec4(aNormal, 0.0))));

	Color = vec3(M_matrix * boned_position);
	TexCoords = aTexCoords;

	gl_Position = transform * boned_position;
}