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

	//equipedWeapon = WeaponFactory::GetDefaultWeapon();
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

	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>() != nullptr && EntityManager::GetInstance()->GetEntity(other->GetOwnerID()) != characterContainer)
	{
		if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->layer != TotemLayer)
		{
			source.Play(audioMaster->GenBuffer("./res/sound/EnemyHit.wav"));
			source.SetVolume(0.1f);
		}
		
		hitObjects.push_back(EntityManager::GetInstance()->GetEntity(other->GetOwnerID()));

		EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>()->GetHit(damage + (equipedWeapon == nullptr || equipedWeapon->durability <= 0 ? 0 : equipedWeapon->bonusDamage));

		if (equipedWeapon != nullptr)
			equipedWeapon->durability--;
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