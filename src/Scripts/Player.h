#pragma once
#include "ScriptableObject.h"
#include "InputHandler.h"
#include "Weapon.h"

class Player : public ScriptableObject
{
public:
	Player();
	~Player();

	std::shared_ptr<Entity> inputHandler;

	std::shared_ptr<Weapon> weapon;

protected:
	virtual void Start();
	virtual void Update();

private:
	void CreateWeapon();
};