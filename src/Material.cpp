#include "Material.h"

Material::Material(Shader* shader) : shader(shader)
{
	
}

Shader* Material::GetShader()
{
	return shader;
}

void Material::SetShader(Shader* shader)
{
	this->shader = shader;
}