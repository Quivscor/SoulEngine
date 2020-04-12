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
