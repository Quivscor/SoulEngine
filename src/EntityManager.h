#pragma once
#include "Core.h"
#include "Entity.h"
#include "Transform.h"

class Entity;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	template <typename T, typename... Args>
	std::shared_ptr<T> CreateEntity(ComponentManager* componentManagerInstance, Args&&... args)
	{
		std::shared_ptr<T> pointer = std::make_shared<T>(componentManagerInstance);
		pointer->SetEntityID(m_NextEntityIndex++);

		pointer->AddComponent<Transform>(std::forward<Args>(args)...);
		m_Entities.emplace_back(std::dynamic_pointer_cast<Entity>(pointer));

		return pointer;
	}

private:
	std::vector<std::shared_ptr<Entity>> m_Entities;
	int m_NextEntityIndex = 0;
};