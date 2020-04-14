#include "Material.h"

Material::Material(Shader* shader) : shader(shader)
{
	color = glm::vec3(1.0f);
}

Shader* Material::GetShader()
{
	return shader;
}

void Material::SetShader(Shader* shader)
{
	this->shader = shader;
}

glm::vec3 Material::GetColor()
{
	return color;
}

void Material::SetColor(glm::vec3 color)
{
	this->color = color;
}

std::vector<Texture> Material::GetTextures()
{
	return textures;
}

void Material::SetTextures(std::vector<Texture> textures)
{
	this->textures = textures;
}