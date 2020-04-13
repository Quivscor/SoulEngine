#include "EntityManager.h"

EntityManager::EntityManager()
{ }

EntityManager::~EntityManager()
{ }

template<typename T>
void EntityManager::CreateEntity()
{
	m_Entities.push_back(T);
}