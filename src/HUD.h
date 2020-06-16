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
	
	

	float sizeX, sizeY;
	float posX, posY;

private:
	Shader* hudshader;
	GLuint activeTexture;
	GLuint textureIdle;
	
	GLuint vao;
};
