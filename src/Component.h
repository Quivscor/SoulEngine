#pragma once

enum ComponentType
{
	None = 0,
	TransformComponent = 1,
	CameraComponent = 2,
	MaterialComponent = 3,
	MeshComponent = 4,
	ColliderMeshComponent = 5,
	ColliderElipseComponent = 6,
	InputComponent = 7,
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