#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Start()
{
	thisEntity->GetComponent<Transform>()->SetLocalRotation(glm::vec3(0, 0, 0));

	currentAnimation = animationIdle;
}

void Player::Update()
{
	if (inputHandler == nullptr)
		return;

	if (isAttacking)
	{
		currentAttackTime += TimeCustom::GetDeltaTime();

		if (currentAttackTime >= attackTime)
		{
			weapon->TurnOffCollider();
			isAttacking = false;
			canMove = true;
		}
	}

	if (isRolling)
	{
		currentRollTime += TimeCustom::GetDeltaTime();

		if (currentRollTime >= rollTime)
		{
			isRolling = false;
			canRoll = true;
		}
	}

	isMoving = false;
	movingFB = 0;
	movingLR = 0;

	if (canMove)
	{
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_W))
		{
			isMoving = true;
			movingFB = 1;
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_S))
		{
			isMoving = true;
			movingFB = -1;
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_A))
		{
			isMoving = true;
			movingLR = -1;
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_D))
		{
			isMoving = true;
			movingLR = 1;
		}

		if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_K))
		{
			if (canRoll && !isAttacking)
			{
				isRolling = true;
				currentRollTime = 0.0f;
				ChangeAnimation(PlayerAnimationRoll);
			}

		}
	}

	Move();

	if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_J))
	{
		if (!isAttacking && !isRolling)
		{
			currentAttackTime = 0.0f;
			isAttacking = true;
			canMove = false;
			weapon->Use();
			ChangeAnimation(PlayerAnimationAttack);
		}
	}

	if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_E))
	{
		Swap();
	}
}

void Player::Move()
{
	if (!isMoving)
	{
		if (!isAttacking && !isRolling)
			ChangeAnimation(PlayerAnimationIdle);

		return;
	}


	CalculateRotation();

	if (currentAnimation != animationRun && !isRolling)
		ChangeAnimation(PlayerAnimationRun);

	thisEntity->GetComponent<Transform>()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 25.0f * (isRolling == true ? 1.5f : 1.0f));
}

void Player::CalculateRotation()
{
	float finalRotation = 0;

	if (movingFB != 0)
	{
		finalRotation = 90 + 90 * movingFB;

		if (movingLR != 0)
		{
			if (finalRotation == 180)
				finalRotation -= 45 * movingLR;
			else
				finalRotation += 45 * movingLR;
		}
	}
	else if (movingLR != 0)
	{
		finalRotation = 90 * movingLR;
	}

	thisEntity->GetComponent<Transform>()->SetLocalRotation(glm::vec3(0, finalRotation, 0));
}

void Player::CreateWeapon()
{

}

void Player::Swap()
{
	if (weaponInRange == nullptr)
		return;

	std::shared_ptr<WeaponStats> weaponTmp = weapon->GetWeapon();
	weapon->SetWeapon(weaponInRange->weapon);
	weaponInRange->weapon = weaponTmp;

	weaponTmp.reset();

	weaponInRange->UpdateWeapon();
}

void Player::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
		weaponInRange = EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>();
}

void Player::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
		weaponInRange = nullptr;
}

void Player::ChangeAnimation(AnimationType type)
{
	Model* model = nullptr;

	switch (type)
	{
	case PlayerAnimationIdle:
		model = animationIdle;
		break;

	case PlayerAnimationRun:
		model = animationRun;
		break;

	case PlayerAnimationAttack:
		model = animationAttack;
		break;

	case PlayerAnimationRoll:
		model = animationRoll;
		break;

	case PlayerAnimationDeath:
		model = animationDeath;
		break;
	}

	if (currentAnimation == model)
		return;

	model->time = 0;

	if (model == animationAttack)
		model->animSpeedModifier = 2.0f;
	else
		model->animSpeedModifier = 1.0f;


	currentAnimation = model;

	thisEntity->GetComponent<Model>()->UseModel(model);
	thisEntity->GetComponent<Mesh>()->SetAll(model->GetMeshes()[0]);
	thisEntity->GetComponent<Mesh>()->setupMeshfBones();
	thisEntity->GetComponent<Mesh>()->material->SetShader(shader);
}