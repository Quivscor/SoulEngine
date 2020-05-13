#pragma once
#include "ScriptableObject.h"
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

	float attackTime = 2.0f;
	float currentAttackTime = 0.0f;
	bool isAttacking = false;
};