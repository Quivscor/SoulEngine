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

	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;

private:
	void CreateWeapon();
	void Swap();

	std::shared_ptr<WeaponOnTheGround> weaponInRange;
};