#include "System.h"

System::System()
{
}


System::~System()
{
}

void System::SetSystemID(int ID)
{
	m_SystemID = ID;
}

void System::RegisterEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}