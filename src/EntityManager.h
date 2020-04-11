#pragma once
#include "Core.h"
#include "Entity.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	template<typename T>
	void CreateEntity();

private:
	std::vector<Entity> m_Entities;
	int m_NextEntityIndex = 0;
};