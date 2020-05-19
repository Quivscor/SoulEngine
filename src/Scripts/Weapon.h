#pragma once
#include "ScriptableObject.h"
#include "WeaponOnTheGround.h"

class Weapon : public ScriptableObject
{
public:
	Weapon();
	~Weapon();

	void Use();

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnTriggerStay(std::shared_ptr<Collider> other) override;

	std::shared_ptr<WeaponStats> GetWeapon();
	void SetWeapon(std::shared_ptr<WeaponStats> newWeapon);

private:
	void TurnOffCollider();
	std::shared_ptr<WeaponStats> equipedWeapon;

	float attackTime = 0.2f;
	float currentAttackTime = 0.0f;
	float damage = 25.0f;
	std::vector<std::shared_ptr<Entity>> hitObjects;
	bool isAttacking = false;
};