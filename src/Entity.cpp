#include "Entity.h"

Entity::Entity()
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
