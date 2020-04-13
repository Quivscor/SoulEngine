#pragma once
#include "Core.h"
#include "Component.h"

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	template <typename T, typename... Args>
	std::unique_ptr<T> AddComponent(int ownerID, Args&& ...)
	{
		std::unique_ptr<T> component = std::make_unique<T>(std::forward(Args&& ...));
		(*component)->SetComponentID(m_NextComponentID++);

		return component;
	}

	template <typename T>
	bool RemoveComponent(int ownerID)
	{

	}

private:
	int m_NextComponentID = 0;
};