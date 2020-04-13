#pragma once
#include "SystemManager.h"
#include "Component.h"

class SystemManager;

class System
{
public:
	friend class SystemManager;

	System();
	~System();

	virtual void Init() const = 0;
	virtual void Update() const = 0;
	virtual void LateUpdate() const = 0;
	virtual int GetSystemID() const { return m_SystemID; }

	bool operator==(const System& obj) const
	{
		return (this->m_SystemID == obj.m_SystemID);
	}

private:
	int m_SystemID;
	std::shared_ptr<SystemManager> m_SystemManagerInstance;
	std::vector<std::shared_ptr<Component>> m_Components;

	void SetSystemID(int ID);
};

struct SystemHash
{
public:
	size_t operator()(const System& t) const {
		return t.GetSystemID();
	}
};