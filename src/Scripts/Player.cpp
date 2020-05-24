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
}

void Player::Update()
{
	if (inputHandler == nullptr)
		return;

	isMoving = false;
	movingFB = 0;
	movingLR = 0;

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

	Move();

	if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_J))
	{
		weapon->Use();
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