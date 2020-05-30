#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Start()
{
	currentAnimation = animationIdle;
}

void Enemy::Update()
{
	if (isTriggered)
	{
		Move();
	}
}

void Enemy::Move()
{
	ChangeAnimation(EnemyAnimationRun);

	thisEntity->GetComponent<Transform>()->GetParent()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 22.0f);
}

void Enemy::ChangeAnimation(EnemyAnimationType type)
{
	Model* model = nullptr;

	switch (type)
	{
	case EnemyAnimationIdle:
		model = animationIdle;
		break;

	case EnemyAnimationRun:
		model = animationRun;
		break;

	case EnemyAnimationAttack:
		model = animationAttack;
		break;

	case EnemyAnimationDeath:
		model = animationDeath;
		break;
	}

	if (currentAnimation == model)
		return;

	model->time = 0;

	currentAnimation = model;

	EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->GetComponent<Model>()->UseModel(model);
	EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->GetComponent<Mesh>()->SetAll(model->GetMeshes()[0]);
	EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->GetComponent<Mesh>()->setupMeshfBones();
	EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->GetComponent<Mesh>()->material->SetShader(shader);
}
