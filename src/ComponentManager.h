#pragma once
#include "Core.h"
#include "Component.h"
#include "Transform.h"

struct PairHash
{
	size_t operator() (std::pair<int, ComponentType> pair) const
	{
		return pair.first ^ pair.second;
	}
};

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
		m_Components.insert({ {ownerID, type}, std::dynamic_pointer_cast<Component>(component) });

		return component;
	}

	template <typename T>
	bool RemoveComponent(int ownerID)
	{

	}

	template <typename T>
	std::shared_ptr<T> GetComponent(int ownerID)
	{
		T temp;
		ComponentType type = temp.GetComponentType();
		auto it = m_Components.find({ ownerID, type });
		return std::dynamic_pointer_cast<T>(it->second);
	}

private:
	int m_NextComponentID = 0;
	std::unordered_map<std::pair<int, ComponentType>, std::shared_ptr<Component>, PairHash> m_Components;
};