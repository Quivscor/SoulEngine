#pragma once
#include "Core.h"

class Mesh;
class Model;

class Shader;
class InstanceManager
{
public:

	struct parameters
	{
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rot;
		glm::mat4 modelMat;
	};

	std::vector <parameters> m_instanceObjects;
	
	int amount = 0;
	glm::mat4 *instanceModels;
	Shader* m_shader;
	Model* m_model;
	Mesh* m_mesh;
	unsigned int buffer;

	InstanceManager(Model* model);
	~InstanceManager();

	void AddParameters(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, glm::mat4 model);

private:
	
	static std::shared_ptr<InstanceManager> m_Instance;
};