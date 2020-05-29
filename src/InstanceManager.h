#pragma once
#include "Core.h"

class Model;

class InstanceManager
{
public:

	struct parameters
	{
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rot;
	};

	std::vector <parameters> m_instanceObjects;
	Model* m_model;

	InstanceManager(Model* model);
	~InstanceManager();

	void AddParameters(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);

private:
	
	static std::shared_ptr<InstanceManager> m_Instance;
};