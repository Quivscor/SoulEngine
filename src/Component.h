#pragma once

enum ComponentType
{
	None = 0,
	TransformComponent = 1
};

class Component
{
public:
	friend class ComponentManager;

	Component();
	~Component();
	int GetOwnerID();
	virtual ComponentType GetComponentType() const { return ComponentType::None; }

protected:
	int m_EntityID;
	int m_ComponentID;
	ComponentType m_Type;

	void SetComponentID(int ID);
	void SetOwnerID(int ID);
};