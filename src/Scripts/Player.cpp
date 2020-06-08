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

	currentAnimation = animationSwordIdle;
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

	if (currentAnimation != animationSwordRun && currentAnimation != animationAxeRun && currentAnimation != animationMaceRun && !isRolling)
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

	glm::vec3 currentRotation = thisEntity->GetComponent<Transform>()->GetGlobalRotation();

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

	glm::vec3 rotation = glm::mix(currentRotation, glm::vec3(0, finalRotation, 0), TimeCustom::GetDeltaTime() * 7.5f);
	thisEntity->GetComponent<Transform>()->SetLocalRotation(rotation);
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

	attackSpeed = weapon->GetWeapon()->bonusSpeed;
	attackTime = 0.5f - ((attackSpeed - 2.0f) / 4.0f);

	std::cout << "Attack speed: " << attackSpeed << std::endl;
	std::cout << "Attack time: " << attackTime << std::endl;

	AnimationType animToSet;

	if (isAttacking)
		animToSet = PlayerAnimationAttack;
	else if (isRolling)
		animToSet = PlayerAnimationRoll;
	else if (isMoving)
		animToSet = PlayerAnimationRun;
	else
		animToSet = PlayerAnimationIdle;

	ChangeAnimation(animToSet);

	weaponComparator->UpdateStats(25, this->weapon->GetWeapon()->bonusDamage, weaponInRange->weapon->bonusDamage, weaponComparator->oldDamage, weaponComparator->newDamage);
	weaponComparator->UpdateStats(2, this->weapon->GetWeapon()->bonusSpeed - 2, weaponInRange->weapon->bonusSpeed - 2, weaponComparator->oldASpeed, weaponComparator->newASpeed);
	weaponComparator->UpdateStats(1, 0, 0, weaponComparator->oldDurability, weaponComparator->newDurability);
}

void Player::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
	{
		weaponInRange = EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>();
		weaponComparator->Show(true);

		weaponComparator->UpdateStats(25, this->weapon->GetWeapon()->bonusDamage, weaponInRange->weapon->bonusDamage, weaponComparator->oldDamage, weaponComparator->newDamage);
		weaponComparator->UpdateStats(2, this->weapon->GetWeapon()->bonusSpeed - 2, weaponInRange->weapon->bonusSpeed - 2, weaponComparator->oldASpeed, weaponComparator->newASpeed);
		weaponComparator->UpdateStats(1, 0, 0, weaponComparator->oldDurability, weaponComparator->newDurability);
	}
}

void Player::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
	{
		weaponInRange = nullptr;
		weaponComparator->Show(false);
	}
}

void Player::ChangeAnimation(AnimationType type)
{
	Model* model = nullptr;

	if (weapon->GetWeapon()->model.type == Sword)
	{
		switch (type)
		{
		case PlayerAnimationIdle:
			model = animationSwordIdle;
			break;

		case PlayerAnimationRun:
			model = animationSwordRun;
			break;

		case PlayerAnimationAttack:
			model = animationSwordAttack;
			break;

		case PlayerAnimationRoll:
			model = animationSwordRoll;
			break;

		case PlayerAnimationDeath:
			model = animationDeath;
			break;
		}
	}
	else if (weapon->GetWeapon()->model.type == Axe)
	{
		switch (type)
		{
		case PlayerAnimationIdle:
			model = animationAxeIdle;
			break;

		case PlayerAnimationRun:
			model = animationAxeRun;
			break;

		case PlayerAnimationAttack:
			model = animationAxeAttack;
			break;

		case PlayerAnimationRoll:
			model = animationAxeRoll;
			break;

		case PlayerAnimationDeath:
			model = animationDeath;
			break;
		}
	}
	else if (weapon->GetWeapon()->model.type == Mace)
	{
		switch (type)
		{
		case PlayerAnimationIdle:
			model = animationMaceIdle;
			break;

		case PlayerAnimationRun:
			model = animationMaceRun;
			break;

		case PlayerAnimationAttack:
			model = animationMaceAttack;
			break;

		case PlayerAnimationRoll:
			model = animationMaceRoll;
			break;

		case PlayerAnimationDeath:
			model = animationDeath;
			break;
		}
	}


	if (currentAnimation == model)
		return;

	model->time = 0;

	if (model == animationSwordAttack || model == animationAxeAttack || model == animationMaceAttack)
		model->animSpeedModifier = attackSpeed;
	else
		model->animSpeedModifier = 1.0f;


	currentAnimation = model;

	thisEntity->GetComponent<Model>()->UseModel(model);
	thisEntity->GetComponent<Mesh>()->SetAll(model->GetMeshes()[0]);
	thisEntity->GetComponent<Mesh>()->setupMeshfBones();
	thisEntity->GetComponent<Mesh>()->material->SetShader(shader);
}