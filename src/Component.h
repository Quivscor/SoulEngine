#pragma once
class Component
{
public:
	Component();
	~Component();
	int GetOwnerID();

private:
	int m_EntityID;
	int m_ComponentID;
};

