#pragma once
#include "Core.h"
#include "System.h"
#include "Entity.h"

struct ControllerInput
{
public: 
	ControllerInput() {}

	float axis1;
	float axis2;
	float axis3;
	float axis4;
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
	static ControllerInput m_ControllerInputQueue;
};

