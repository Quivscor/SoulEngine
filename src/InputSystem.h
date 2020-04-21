#pragma once
#include "Core.h"
#include "System.h"
#include "Entity.h"

struct InputHolder
{
public: 
	std::unordered_set<std::pair<int,int>> keycodes;
};

class InputSystem : public System
{
public:
	friend class InputHandler;

	InputSystem();
	~InputSystem();
	virtual void Init() const {};
	virtual void Update() const;
	virtual void LateUpdate() const;
	static void AddInput(int input, int action);
	bool GetKeyUp(int input);
	bool GetKeyDown(int input);
	SystemType GetSystemType() const override { return SystemType::Input; }

	static std::set<std::pair<int, int>> m_KeyboardInputQueue;
};

