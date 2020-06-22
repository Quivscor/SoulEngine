#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 3) in ivec4 bone_ids;     // INT pointer
layout(location = 4) in vec4 weights;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;
const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];
uniform bool anim;
void main()
{


	
if(anim==true)
{
mat4 bone_transform = bones[bone_ids[0]] * weights[0];
		bone_transform += bones[bone_ids[1]] * weights[1];
		bone_transform += bones[bone_ids[2]] * weights[2];
		bone_transform += bones[bone_ids[3]] * weights[3];
			
	vec4 boned_position = bone_transform * vec4(aPos, 1.0); // transformed by bones
 gl_Position = lightSpaceMatrix * model  * boned_position;
}
else
   gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0); 
}