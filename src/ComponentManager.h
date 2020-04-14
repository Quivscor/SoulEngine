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
		std::shared_ptr<T> component = std::make_shared<T>(std::forward(Args&& ...));
		(*component)->SetComponentID(m_NextComponentID++);
		m_Components.insert((*component)->GetComponentType(), component);

		return component;
	}

	template <typename T>
	bool RemoveComponent(int ownerID)
	{

	}

private:
	int m_NextComponentID = 0;
	std::unordered_map<ComponentType, std::shared_ptr<Component>> m_Components;
};