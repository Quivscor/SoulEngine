#include "Component.h"

Component::Component()
{
}


Component::~Component()
{
}

int Component::GetOwnerID()
{
	return m_EntityID;
}

void Component::SetComponentID(int ID)
{
	m_ComponentID = ID;
}

void Component::SetOwnerID(int ID)
{
	m_EntityID = ID;
}
