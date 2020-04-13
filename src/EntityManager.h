#pragma once
#include "Core.h"
#include "Entity.h"

class Entity;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	template <typename T, typename... Args>
	void CreateEntity(Args&&... args)
	{
		std::shared_ptr<T> pointer = std::make_unique<T>(std::forward<Args>(args)...);
		(*pointer)->SetEntityID(m_NextEntityIndex++);

		m_Entities.emplace_back(std::dynamic_pointer_cast<Entity>(pointer));
	}

private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
	int m_NextEntityIndex = 0;
};