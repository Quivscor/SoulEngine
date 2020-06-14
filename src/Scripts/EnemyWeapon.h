#pragma once
#include "ScriptableObject.h"
#include "Character.h"

class EnemyWeapon : public ScriptableObject
{

public:
	EnemyWeapon();
	~EnemyWeapon();

	void Use();
	void TurnOffCollider();

	virtual void Start() override;
	//virtual void Update() override;
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;

	std::shared_ptr<Entity> player;

	void DealDmg(std::shared_ptr<Entity> target);

private:
	//[SerializeField] F [*]
	float damage = 10.0f;
};