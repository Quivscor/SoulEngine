#pragma once
#include "ComponentManager.h"

enum Layer
{
	DefaultLayer,
	PlayerLayer,
	EnemyLayer,
	GroundLayer,
	WaterLayer,
	TotemLayer
};

class Entity
{
public:
	friend class EntityManager;

	Entity();
	~Entity();
	int GetEntityID();

	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args)
	{
		return ComponentManager::GetInstance()->AddComponent<T>(m_EntityID, std::forward<Args>(args)...);
	}

	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		return ComponentManager::GetInstance()->GetComponent<T>(m_EntityID);
	}

	bool isActive = true;
	Layer layer = DefaultLayer;

	/*template <typename T>
	std::vector<std::shared_ptr<T>> GetComponents()
	{
		return ComponentManager::GetInstance()->GetComponents<T>(m_EntityID);
	}*/

protected:
	int m_EntityID;
	bool m_IsActive = true;

	void SetEntityID(int ID);
};

