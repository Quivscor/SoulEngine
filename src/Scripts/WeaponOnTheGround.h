#pragma once
#include "ScriptableObject.h"
#include "Character.h"
#include "WeaponFactory.h"

class WeaponOnTheGround : public ScriptableObject
{
public:
	WeaponOnTheGround();
	~WeaponOnTheGround();

protected:
	virtual void Start() override;
	virtual void Update() override;

private:
	float rotatingSpeed = 15.0f;
	std::shared_ptr<WeaponStats> weapon;
};