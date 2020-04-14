#include "SystemManager.h"

SystemManager::SystemManager()
{ }

SystemManager::~SystemManager()
{ }

void SystemManager::UpdateAll()
{
	for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
	{
		(*it)->Update();
	}
}

void SystemManager::LateUpdateAll()
{
	for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
	{
		(*it)->LateUpdate();
	}
}