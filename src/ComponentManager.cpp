#include "ComponentManager.h"

std::shared_ptr<ComponentManager> ComponentManager::m_Instance = 0;

ComponentManager::ComponentManager()
{ }

ComponentManager::~ComponentManager()
{ }

std::shared_ptr<ComponentManager> ComponentManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		ComponentManager::m_Instance = std::make_shared<ComponentManager>();
	}

	return ComponentManager::m_Instance;
}