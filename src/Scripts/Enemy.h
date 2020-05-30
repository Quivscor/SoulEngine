#pragma once
#include "ScriptableObject.h"
#include "Model.h"

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

protected:
	virtual void Start() override;
	virtual void Update() override;

private:
	void Move();
	void ChangeAnimation(EnemyAnimationType type);

	bool isTriggered = false;
};