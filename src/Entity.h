#pragma once
#include "ComponentManager.h"

class Entity
{
public:
	friend class EntityManager;

	Entity(ComponentManager* componentManagerInstance);
	~Entity();
	int GetEntityID();

	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&& ...)
	{
		return m_ComponentManagerInstance->AddComponent<T>(m_EntityID, std::forward<Args>(args)...);
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		return m_ComponentManagerInstance->GetComponent<T>(m_EntityID);
	}

protected:
	int m_EntityID;
	bool m_IsActive = true;
	ComponentManager* m_ComponentManagerInstance;

	void SetEntityID(int ID);
};

