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
		float distance = glm::sqrt(glm::pow(thisEntity->GetComponent<Transform>()->GetGlobalPosition().x - playerPosition->GetGlobalPosition().x, 2) + glm::pow(thisEntity->GetComponent<Transform>()->GetGlobalPosition().z - playerPosition->GetGlobalPosition().z, 2));

		if (distance > minimumDistanceToPlayer)
		{
			CalculateRotation();
			Move();
		}
		else
			Attack();
	}
}

void Enemy::Attack()
{
	ChangeAnimation(EnemyAnimationAttack);
}

void Enemy::Move()
{
	ChangeAnimation(EnemyAnimationRun);

	thisEntity->GetComponent<Transform>()->GetParent()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 22.0f);
}

void Enemy::CalculateRotation()
{
	int diffUD = thisEntity->GetComponent<Transform>()->GetGlobalPosition().z - playerPosition->GetGlobalPosition().z;
	int diffLR = thisEntity->GetComponent<Transform>()->GetGlobalPosition().x - playerPosition->GetGlobalPosition().x;

	if (diffLR < 0)
		diffLR = 1;
	else if (diffLR > 0)
		diffLR = -1;
	else
		diffLR = 0;

	if (diffUD < 0)
		diffUD = -1;
	else if (diffUD > 0)
		diffUD = 1;
	else
		diffUD = 0;

	float finalRotation = 0;

	if (diffUD != 0)
	{
		finalRotation = 90 + 90 * diffUD;

		if (diffLR != 0)
		{
			if (finalRotation == 180)
				finalRotation -= 45 * diffLR;
			else
				finalRotation += 45 * diffLR;
		}
	}
	else if (diffLR != 0)
	{
		finalRotation = 90 * diffLR;
	}

	thisEntity->GetComponent<Transform>()->GetParent()->SetLocalRotation(glm::vec3(0, finalRotation, 0));
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
