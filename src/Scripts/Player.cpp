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

	currentAnimation = animationRun;
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
	}

	Move();

	if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_J))
	{
		if (!isAttacking)
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
		return;

	CalculateRotation();

	if (currentAnimation != animationRun)
		ChangeAnimation(PlayerAnimationRun);

	thisEntity->GetComponent<Transform>()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 25.0f);
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
		model->time = 0;
		break;

	case PlayerAnimationRun:
		model = animationRun;
		model->time = 0;
		break;

	case PlayerAnimationAttack:
		model = animationAttack;
		model->time = 0;
		break;
	}

	currentAnimation = model;

	thisEntity->GetComponent<Model>()->UseModel(model);
	thisEntity->GetComponent<Mesh>()->SetAll(model->GetMeshes()[0]);
	thisEntity->GetComponent<Mesh>()->setupMeshfBones();
	thisEntity->GetComponent<Mesh>()->material->SetShader(shader);
}