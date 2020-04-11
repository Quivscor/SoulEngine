#pragma once
#include "ComponentManager.h"

class Entity
{
public:
	Entity(int ID);
	~Entity();
	int GetEntityID();

private:
	int m_EntityID;
	ComponentManager* m_ComponentManagerInstance;
};

