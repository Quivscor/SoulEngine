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
	virtual void Init();
	virtual void Update() const;
	virtual void LateUpdate() const;
	static void AddInput(int input, int action);
	static void AddRelease(int input);
	bool GetKeyUp(int input);
	bool GetKeyDown(int input);
	SystemType GetSystemType() const override { return SystemType::Input; }

	static std::set<std::pair<int, int>> m_KeyboardInputQueue;
	static std::set<std::pair<int, int>> m_KeyboardInputQueueLastFrame;
	static std::vector<int> m_ReleaseQueue;
	static ControllerInput m_ControllerInputQueue;
};

