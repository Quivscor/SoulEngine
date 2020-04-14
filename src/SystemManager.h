#pragma once
#include "Core.h"
#include "System.h"
#include "IEventListener.h"
#include "Event.h"
#include "Component.h"

class System;

class SystemManager
{
public:
	SystemManager();
	~SystemManager();

	//Method for creating system instances, regardless of what arguments the particular system constructor needs.
	template <typename T, typename... Args>
	void AddSystem(Args&&... args)
	{
		std::shared_ptr<T> pointer = std::make_shared<T>(std::forward<Args>(args)...);
		(*pointer)->SetSystemID(m_NextSystemIndex++);

		m_Systems.emplace_back(std::dynamic_pointer_cast<System>(pointer));
	}

	void UpdateAll();
	void LateUpdateAll();

private:
	int m_NextSystemIndex = 0;
	std::vector<std::shared_ptr<System>> m_Systems;
};