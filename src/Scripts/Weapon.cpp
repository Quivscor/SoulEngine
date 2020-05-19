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

	equipedWeapon = WeaponFactory::GetDefaultWeapon();
}

void Weapon::Update()
{
	if (isAttacking)
	{
		currentAttackTime += TimeCustom::GetDeltaTime();

		if (currentAttackTime >= attackTime)
		{
			TurnOffCollider();
		}
	}
}

void Weapon::OnTriggerStay(std::shared_ptr<Collider> other)
{
	for (int i = 0; i < hitObjects.size(); i++)
		if (hitObjects[i] != nullptr && EntityManager::GetInstance()->GetEntity(other->GetOwnerID()) == hitObjects[i])
			return;

	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>() != nullptr)
	{
		hitObjects.push_back(EntityManager::GetInstance()->GetEntity(other->GetOwnerID()));

		EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>()->GetHit(damage + equipedWeapon->bonusDamage);
	}
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
	hitObjects.clear();
	isAttacking = false;
	thisEntity->GetComponent<Collider>()->enabled = false;
}

std::shared_ptr<WeaponStats> Weapon::GetWeapon()
{
	return equipedWeapon;
}

void Weapon::SetWeapon(std::shared_ptr<WeaponStats> newWeapon)
{
	equipedWeapon = newWeapon;
}