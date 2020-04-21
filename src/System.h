#pragma once
#include "SystemManager.h"
#include "Entity.h"

class SystemManager;

enum class SystemType
{
	None = 0,
	Renderer, Physics, Input
};

class System
{
public:
	friend class SystemManager;
	friend class ComponentManager;

	System();
	~System();

	virtual void Init() const = 0;
	virtual void Update() const = 0;
	virtual void LateUpdate() const = 0;
	virtual int GetSystemID() const { return m_SystemID; }
	virtual SystemType GetSystemType() const = 0;

	bool operator==(const System& obj) const
	{
		return (this->m_SystemID == obj.m_SystemID);
	}

	void RegisterEntity(std::shared_ptr<Entity> entity);

protected:
	int m_SystemID;
	std::vector<std::shared_ptr<Entity>> m_Entities;
	std::shared_ptr<SystemManager> m_SystemManagerInstance;

	void SetSystemID(int ID);
};