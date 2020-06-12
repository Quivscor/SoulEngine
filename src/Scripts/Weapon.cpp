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
	//std::cout << thisEntity->GetComponent<Transform>()->GetGlobalPosition().x << " " << thisEntity->GetComponent<Transform>()->GetGlobalPosition().y << " " << thisEntity->GetComponent<Transform>()->GetGlobalPosition().z << std::endl;
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
	thisEntity->GetComponent<Collider>()->enabled = true;
}

void Weapon::TurnOffCollider()
{
	hitObjects.clear();
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