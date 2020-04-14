#pragma once

enum class ComponentType
{
	None = 0,
	Transform
};

class Component
{
public:
	Component();
	~Component();
	int GetOwnerID();
	virtual ComponentType GetComponentType() const { return ComponentType::None; }

private:
	int m_EntityID;
	int m_ComponentID;
	ComponentType m_Type;
};

