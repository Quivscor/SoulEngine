#pragma once
#include "ScriptableObject.h"
#include "InputHandler.h"

class Player : public ScriptableObject
{
public:
	Player();
	~Player();

	std::shared_ptr<Entity> inputHandler;

protected:
	virtual void Start();
	virtual void Update();
};