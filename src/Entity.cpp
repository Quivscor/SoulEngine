#include "Entity.h"

Entity::Entity(ComponentManager* componentManagerInstance) : m_ComponentManagerInstance(componentManagerInstance)
{
}

Entity::~Entity()
{
}

int Entity::GetEntityID()
{
	return m_EntityID;
}

void Entity::SetEntityID(int ID)
{
	m_EntityID = ID;
}
