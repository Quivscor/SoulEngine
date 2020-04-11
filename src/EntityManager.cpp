#include "EntityManager.h"

template<typename T>
void EntityManager::CreateEntity()
{
	m_Entities.push_back(T);
}