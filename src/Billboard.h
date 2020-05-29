
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
	static Billboard* Instance(const char* imagepath)
	{
		static Billboard* inst =  new Billboard( imagepath);
		return inst;
	}

	Billboard(const char* imagepath);
	~Billboard();
	void Draw( std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size);
	Shader* shaderbil;
	GLuint loadDDS(const char* imagepath);
	GLuint Texture;
private:
	// texture;
	GLuint vao;
};
