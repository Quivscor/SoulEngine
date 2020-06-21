
#pragma once
#include <glad/glad.h>
#include<Core.h>
#include <map>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "stb_image.h"
class Billboard : public Component
{
public:
	Billboard();
	~Billboard();

	void setLife(float Life);
	Billboard(char* imagepath, bool x);

	GLuint loadDDS(const char* imagepath);
	void Draw( std::shared_ptr<Entity>  camera, glm::vec3 position, glm::vec2 size);
	void SetBillboard(char* imagepath, bool x);
	Shader* shaderbil;
	bool type= true;
	virtual ComponentType GetComponentType() const override { return ComponentType::BillboardComponent; }

	float LifeLevel;
	GLuint Texture;
	GLuint billboard_vertex_buffer;

private:
	// texture;
	GLuint vao;
};

