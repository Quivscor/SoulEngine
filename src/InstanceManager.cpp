#include "InstanceManager.h"

InstanceManager::InstanceManager(Model* model)
{
	m_model = model;
}

InstanceManager::~InstanceManager()
{
}

void InstanceManager::AddParameters(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
{
	parameters param;
	param.pos = pos;
	param.scale = scale;
	param.rot = rot;
	m_instanceObjects.push_back(param);
}
