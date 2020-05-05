#include "EntityManager.h"

std::shared_ptr<EntityManager> EntityManager::m_Instance = 0;

EntityManager::EntityManager()
{ }

EntityManager::~EntityManager()
{ }

std::shared_ptr<EntityManager> EntityManager::GetInstance()
{
	if (EntityManager::m_Instance == NULL)
	{
		EntityManager::m_Instance = std::make_shared<EntityManager>();
	}

	return EntityManager::m_Instance;
}