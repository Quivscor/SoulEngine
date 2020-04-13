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

	}

private:
};