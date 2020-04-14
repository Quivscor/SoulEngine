#pragma once
#include "Component.h"
#include "Shader.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Material : Component
{
public:
	Material(Shader* shader);
	~Material();

	Shader* GetShader();
	void SetShader(Shader* shader);
	std::vector<Texture> GetTextures();
	void SetTextures(std::vector<Texture> textures);
	glm::vec3 GetColor();
	void SetColor(glm::vec3 color);

private:
	Shader* shader;
	glm::vec3 color;
	std::vector<Texture> textures;
	//Texture texture; //????
};