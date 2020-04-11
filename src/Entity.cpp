#include "Entity.h"

Entity::Entity(int ID) : m_EntityID(ID)
{
}

Entity::~Entity()
{
}

int Entity::GetEntityID()
{
	return m_EntityID;
}
