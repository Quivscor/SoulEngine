#include "SystemManager.h"

SystemManager::SystemManager()
{ }

SystemManager::~SystemManager()
{ }

void SystemManager::UpdateAll()
{
	for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
	{
		(*it)->Update();
	}
}

void SystemManager::LateUpdateAll()
{
	for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
	{
		(*it)->LateUpdate();
	}
}

void SystemManager::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<BindTransform>(BIND_EVENT_FN(SystemManager::BindTransformComponent));
}

bool SystemManager::BindTransformComponent(BindTransform& e)
{
	for (auto it = m_Systems.begin(); it != m_Systems.end(); it++)
	{
		if ((*it)->GetSystemType() == SystemType::Physics)
		{
			std::shared_ptr<Transform> transform = e.GetTransformComponent();
			(*it)->m_Components.emplace_back(std::dynamic_pointer_cast<Component>(transform));
			return true;
		}
	}
	return false;
}