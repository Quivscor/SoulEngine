#pragma once
#include<Core.h>
#include <string>
#include <vector>
#include<Shader.h>
class HUD
{
public:
	HUD(float sizeX, float sizeY, float posX, float posY, const char* textureIdle);
	~HUD();
	void Draw();
	
	float sizeX = 0, sizeY = 0;
	float posX = 0, posY = 0;

private:
	Shader* hudshader = nullptr;
	GLuint activeTexture = 0;
	GLuint textureIdle = 0;
	GLuint vao = 0;
};
