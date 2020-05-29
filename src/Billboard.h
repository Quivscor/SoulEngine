
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
	static Billboard* Instance()
	{
		static Billboard inst;
		return &inst;
	}

	Billboard();
	~Billboard();
	void Draw( std::shared_ptr<Entity>  camera, glm::vec3 position);
	Shader* shaderbil;
private:
	GLuint texture;
	GLuint vao;
};
