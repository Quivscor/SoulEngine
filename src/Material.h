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

private:
	Shader* shader;
	//Texture texture; //????
};