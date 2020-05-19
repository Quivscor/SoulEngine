#pragma once
#ifndef WeaponOntheGround 
#include "ScriptableObject.h"
#include "Character.h"
#include "WeaponFactory.h"

class WeaponOnTheGround : public ScriptableObject
{
public:
	WeaponOnTheGround();
	~WeaponOnTheGround();

	virtual ComponentType GetComponentType() const override { return ComponentType::WeaponOnTheGroundComponent; }
	virtual void Start() override;
	std::shared_ptr<WeaponStats> weapon;

	void UpdateWeapon();

protected:


private:
	float rotatingSpeed = 15.0f;
};

#endif