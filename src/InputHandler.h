#pragma once
#include "Core.h"
#include "Component.h"

class InputHandler : public Component
{
public:
	InputHandler();
	~InputHandler();
	virtual ComponentType GetComponentType() const override { return ComponentType::InputComponent; }
	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKeyRepeat(int key);

	std::set<std::pair<int, int>> m_Keyboard;
private:
	bool CheckKey(int key, int action);
};

