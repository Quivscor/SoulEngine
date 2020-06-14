#pragma once
#include "ScriptableObject.h"
#include "Model.h"
#include "EnemyWeapon.h"

enum EnemyAnimationType
{
	EnemyAnimationIdle,
	EnemyAnimationRun,
	EnemyAnimationAttack,
	EnemyAnimationDeath
};

class Enemy : public ScriptableObject
{
	friend class Village;

public:
	Enemy();
	~Enemy();

	std::shared_ptr<Transform> playerPosition;

	Shader* shader;

	Model* currentAnimation;
	Model* animationIdle;
	Model* animationAttack;
	Model* animationRun;
	Model* animationDeath;

	std::shared_ptr<Entity> weapon;

protected:
	virtual void Start() override;
	virtual void Update() override;

private:
	void Move();
	void Attack();
	void ChangeAnimation(EnemyAnimationType type);
	void CalculateRotation();

	float attackTime = 1.0f;
	float currentAttackTime = 0.0f;
	bool isAttacking = false;

	bool isTriggered = false;
	float minimumDistanceToPlayer = 2.0f;
};