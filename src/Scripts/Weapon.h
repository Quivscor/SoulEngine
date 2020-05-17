#pragma once
#include "ScriptableObject.h"
#include "Character.h"

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

private:
	void TurnOffCollider();

	float attackTime = 0.2f;
	float currentAttackTime = 0.0f;
	float damage = 25.0f;
	std::vector<std::shared_ptr<Entity>> hitObjects;
	bool isAttacking = false;
};