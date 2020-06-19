#pragma once

enum ComponentType
{
	None = 0,
	TransformComponent = 1,
	CameraComponent = 2,
	MaterialComponent = 3,
	MeshComponent = 4,
	InputComponent = 5,
	ColliderComponent = 6,
	ScriptableObjectComponent = 7,
	CharacterComponent = 8,
	WeaponOnTheGroundComponent = 9,
	WaterComponent = 10,
	TextComponent = 11,
	BillboardComponent = 12,
	HUDComponent = 13
};

class Component
{
public:
	friend class ComponentManager;

	Component();
	~Component();
	int GetOwnerID();
	virtual ComponentType GetComponentType() const { return ComponentType::None; }

	bool enabled = true;

protected:
	int m_EntityID;
	int m_ComponentID;
	ComponentType m_Type;

	void SetComponentID(int ID);
	void SetOwnerID(int ID);
};