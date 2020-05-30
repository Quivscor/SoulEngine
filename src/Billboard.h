
#pragma once
#include <glad/glad.h>

#include <map>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include<TimeCustom.h>
class Billboard
{
public:
	static Billboard* Instance( )
	{
		static Billboard inst ;
		return &inst;
	}

	Billboard( );
	~Billboard();
	void Draw(char* imagepath, std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size, bool x);
	Shader* shaderbil;
	GLuint loadDDS(const char* imagepath);
	//char* imagepath;
	GLuint Texture;
	GLuint billboard_vertex_buffer;
private:
	// texture;
	GLuint vao;
};
