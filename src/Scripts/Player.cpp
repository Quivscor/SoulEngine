#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Start()
{

}

void Player::Update()
{
	if (inputHandler == nullptr)
		return;

	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_A))
	{
		thisEntity->GetComponent<Transform>()->Rotate(Transform::Up() * (float)TimeCustom::GetDeltaTime() * 50.0f);
	}
	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_D))
	{
		thisEntity->GetComponent<Transform>()->Rotate(Transform::Up() * (float)TimeCustom::GetDeltaTime() * -50.0f);
	}
	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_W))
	{
		thisEntity->GetComponent<Transform>()->Move(Transform::Forward() * (float)TimeCustom::GetDeltaTime() * 25.0f);
	}
	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_S))
	{
		thisEntity->GetComponent<Transform>()->Move(Transform::Back() * (float)TimeCustom::GetDeltaTime() * 25.0f);
	}

	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_J))
	{
		weapon->Use();
	}

	if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_E))
	{
		Swap();
	}
}

void Player::CreateWeapon()
{

}

void Player::Swap()
{
	if (weaponInRange == nullptr)
		return;
}

void Player::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
		std::cout << "Weapon in range yes!" << std::endl;
}

void Player::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<WeaponOnTheGround>() != nullptr)
		std::cout << "Weapon in range no!" << std::endl;
}