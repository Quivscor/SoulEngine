#pragma once
#include "SystemManager.h"

class System
{
public:
	friend class SystemManager;

	System(int ID);
	~System();

	virtual void Update() const = 0;
	virtual void LateUpdate() const = 0;
	virtual int GetSystemID() const { return m_SystemID; }

	bool operator==(const System& obj) const
	{
		if (m_SystemID == obj.m_SystemID)
			return true;
		else
			return false;
	}

	bool operator!=(const System& obj) const
	{
		if (m_SystemID != obj.m_SystemID)
			return true;
		else
			return false;
	}

private:
	int m_SystemID;
	SystemManager* m_SystemManagerInstance;
};

namespace std
{
	template<>
	struct hash<System>
	{
		size_t operator()(const System & obj) const
		{
			return hash<int>()(obj.GetSystemID());
		}
	};
}