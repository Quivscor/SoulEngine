#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Start()
{
	currentAnimation = animationIdle;
}

void Enemy::Update()
{
	if (isAttacking)
	{
		currentAttackTime += TimeCustom::GetDeltaTime();

		if (currentAttackTime >= attackTime)
		{
			isAttacking = false;
			//thisEntity->GetComponent<Collider>()->isStatic = false;
			weapon->GetComponent<EnemyWeapon>()->TurnOffCollider();

			std::shared_ptr<Entity> playerRef = weapon->GetComponent<EnemyWeapon>()->player;

			if (playerRef != nullptr)
			{
				weapon->GetComponent<EnemyWeapon>()->DealDmg(playerRef);
			}
		}
	}

	if (isTriggered && !isAttacking)
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
	
	if (!isAttacking)
	{
		isAttacking = true;
		currentAttackTime = 0.0f;
		
		weapon->GetComponent<EnemyWeapon>()->Use();
	}
}

void Enemy::Move()
{
	ChangeAnimation(EnemyAnimationRun);

	thisEntity->GetComponent<Transform>()->GetParent()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 22.0f * runningSpeed);
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

	glm::vec3 currentRotation = thisEntity->GetComponent<Transform>()->GetParent()->GetGlobalRotation();

	float distanceIncreased = glm::abs(currentRotation.y + 360 - finalRotation);
	float distanceDecreased = glm::abs(currentRotation.y - 360 - finalRotation);
	float originalDistanse = glm::abs(currentRotation.y - finalRotation);

	if (distanceIncreased < originalDistanse || distanceDecreased < originalDistanse)
	{
		if (distanceDecreased < distanceIncreased)
			currentRotation.y -= 360;
		else
			currentRotation.y += 360;
	}

	glm::vec3 rotation = glm::mix(currentRotation, glm::vec3(0, finalRotation, 0), TimeCustom::GetDeltaTime() * 5.0f);
	thisEntity->GetComponent<Transform>()->GetParent()->SetLocalRotation(rotation);
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
