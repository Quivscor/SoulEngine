#include "InstanceManager.h"

InstanceManager::InstanceManager(Model* model)
{
	m_model = model;
}

InstanceManager::~InstanceManager()
{
}

void InstanceManager::AddParameters(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, glm::mat4 model)
{
	parameters param;
	param.pos = pos;
	param.scale = scale;
	param.rot = rot;
	param.modelMat = model;
	m_instanceObjects.push_back(param);
}
