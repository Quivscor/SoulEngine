
#pragma once
#include <glad/glad.h>
#include<Core.h>
#include <map>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include<TimeCustom.h>
class Billboard
{
public:
	static void Instance(char* imagepath, bool x, std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size)
	{
		 Billboard *inst = new Billboard(imagepath,  x) ;
		 inst->Draw( camera, position,  size);
			 delete inst;
	}
	Billboard(char* imagepath, bool x);
	Billboard( );
	~Billboard();
	void Draw( std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size);
	Shader* shaderbil;
	GLuint loadDDS(const char* imagepath);
	//char* imagepath;
	GLuint Texture;
	GLuint billboard_vertex_buffer;
private:
	// texture;
	GLuint vao;
};
