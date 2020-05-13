#include "Weapon.h"

Weapon::Weapon()
{

}

Weapon::~Weapon()
{

}

void Weapon::Start()
{
	thisEntity->GetComponent<Collider>()->enabled = false;
}

void Weapon::Update()
{
	if (isAttacking)
	{
		currentAttackTime += Time::GetDeltaTime();

		if (currentAttackTime >= attackTime)
		{
			TurnOffCollider();
		}
	}
}

void Weapon::OnTriggerStay(std::shared_ptr<Collider> other)
{
	std::cout << "Imagine how wonderful is this dmg" << std::endl;
}

void Weapon::Use()
{
	if (isAttacking)
		return;

	isAttacking = true;
	currentAttackTime = 0.0f;
	thisEntity->GetComponent<Collider>()->enabled = true;
}

void Weapon::TurnOffCollider()
{
	isAttacking = false;
	thisEntity->GetComponent<Collider>()->enabled = false;
}