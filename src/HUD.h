#ifndef HUD_HEADER
#define HUD_HEADER
#pragma once
#include<Core.h>
#include <string>
#include <vector>
#include<Shader.h>
#include <glad/glad.h>
#include<Core.h>
#include <map>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"
class HUD :  public Component
{
public:

	HUD();
	~HUD();
	void SetHud(float sizeX, float sizeY, float posX, float posY, const char* textureIdle);

	void setLife(float Life);
	bool horizontal = true;
	bool fill = false;
	void Draw();
	void Drawbar();
	virtual ComponentType GetComponentType() const override { return ComponentType::HUDComponent; }
	float sizeX = 0, sizeY = 0;
	float posX = 0, posY = 0;
	HUD(float sizeX, float sizeY, float posX, float posY, const char* textureIdle);
	void setColor(glm::vec4 color);
	void setFill(bool filler);
	void sethorizontal(bool horizon);

private:
	glm::vec4 Color=glm::vec4(0.0f, 0.0f, 0.0f,1.0f);
	Shader* hudshader = nullptr;
	GLuint activeTexture = 0;
	float health= 0.0f;
	GLuint vao = 0;
};
#endif