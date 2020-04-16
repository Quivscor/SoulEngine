#pragma once
#include "Component.h"
#include "Shader.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Material : public Component
{
public:
	Material();
	~Material();

	Shader* GetShader();
	void SetShader(Shader* shader);
	std::vector<Texture> GetTextures();
	void SetTextures(std::vector<Texture> textures);
	glm::vec3 GetColor();
	void SetColor(glm::vec3 color);

	virtual ComponentType GetComponentType() const override { return ComponentType::MaterialComponent; }

private:
	Shader* shader;
	glm::vec3 color;
	std::vector<Texture> textures;
	//Texture texture; //????
};