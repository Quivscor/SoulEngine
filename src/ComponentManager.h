#pragma once
#include "Core.h"
#include "Component.h"

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(int ownerID, Args&& ...)
	{
		std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
		component->SetComponentID(m_NextComponentID++);
		component->SetOwnerID(ownerID);
		ComponentType type = component->GetComponentType();
		m_Components.insert({ type, std::dynamic_pointer_cast<Component>(component) });

		return component;
	}

	template <typename T>
	bool RemoveComponent(int ownerID)
	{

	}

private:
	int m_NextComponentID = 0;
	std::unordered_map<ComponentType, std::shared_ptr<Component>, std::hash<int>> m_Components;
};