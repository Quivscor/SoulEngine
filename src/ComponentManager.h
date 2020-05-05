#pragma once
#include "Core.h"
#include "Component.h"
#include "Transform.h"
#include "InputHandler.h"

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


	static std::shared_ptr<ComponentManager> GetInstance();

	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(int ownerID, Args&&... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>();
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

		if (it == m_Components.end())
		{
			return nullptr;
		}

		return std::dynamic_pointer_cast<T>(it->second);
	}

	/*template <typename T>
	std::vector<std::shared_ptr<T>> GetComponents(int ownerID)
	{
		T temp;
		ComponentType type = temp.GetComponentType();
		std::vector<std::shared_ptr<T>> componentsOfGivenType;

		auto it = m_Components.begin();

		while (it != m_Components.end())
		{
			if (it->first.first == ownerID && it->first.second == type)
				componentsOfGivenType.push_back(std::dynamic_pointer_cast<T>(it->second));

			if (it->first.second == type)
				std::cout << type << std::endl;

			it++;
		}

		std::cout << componentsOfGivenType.size() << std::endl;

		return componentsOfGivenType;
	}*/

private:
	static std::shared_ptr<ComponentManager> m_Instance;

	int m_NextComponentID = 0;
	std::unordered_map<std::pair<int, ComponentType>, std::shared_ptr<Component>, PairHash> m_Components;
};