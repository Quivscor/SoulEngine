#pragma once
#include "Component.h"
#include "Shader.h"

class Material : Component
{
public:
	Material(Shader* shader);
	~Material();

	Shader* GetShader();
	void SetShader(Shader* shader);
	//Texture GetTexture();
	//void SetTexture(Texture texture);
	glm::vec3 GetColor();
	void SetColor(glm::vec3 color);

private:
	Shader* shader;
	glm::vec3 color;
	//Texture texture; //????
};