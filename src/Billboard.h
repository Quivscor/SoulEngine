#pragma once
#include "Core.h"
#include "Shader.h"
#include <iostream>
#include <map>
#include <string>
#include <AssetManager.h>
#include<Camera.h>
class Billboard
{
public:
	static Billboard* Instance()
	{
		static Billboard inst;
		return &inst;
	}

	void drawbill(std::shared_ptr<Entity> cam);
	GLuint CameraRight_worldspace_ID ;
	GLuint CameraUp_worldspace_ID ;
	GLuint ViewProjMatrixID;
	GLuint BillboardPosID ;
	GLuint BillboardSizeID ;
	GLuint LifeLevelID;
	Shader* shaderbil;
	GLuint TextureID ;
	GLuint loadDDS(std::string path);
	static const GLfloat g_vertex_buffer_data[];
	GLuint billboard_vertex_buffer;
	GLuint Texture;
private:
	Billboard();
	~Billboard();

};

