#pragma once
#include "ComponentManager.h"

class Entity
{
public:
	friend class EntityManager;

	Entity(ComponentManager* componentManagerInstance);
	~Entity();
	int GetEntityID();

private:
	int m_EntityID;
	bool m_IsActive = true;
	ComponentManager* m_ComponentManagerInstance;

	void SetEntityID(int ID);
};

