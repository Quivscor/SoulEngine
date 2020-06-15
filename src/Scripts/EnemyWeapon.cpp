#include "EnemyWeapon.h"

EnemyWeapon::EnemyWeapon()
{

}

EnemyWeapon::~EnemyWeapon()
{

}

void EnemyWeapon::TurnOffCollider()
{
	thisEntity->GetComponent<Collider>()->enabled = true;
}

void EnemyWeapon::Use()
{
	player = nullptr;
	thisEntity->GetComponent<Collider>()->enabled = true;
}

void EnemyWeapon::Start()
{
	thisEntity->GetComponent<Collider>()->enabled = false;
}

/*void EnemyWeapon::Update()
{

}*/

void EnemyWeapon::OnTriggerStay(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>() != nullptr && EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->layer == PlayerLayer)
	{
		player = EntityManager::GetInstance()->GetEntity(other->GetOwnerID());
	}
}

void EnemyWeapon::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Character>() != nullptr && EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->layer == PlayerLayer)
	{
		player = nullptr;
	}
}

void EnemyWeapon::DealDmg(std::shared_ptr<Entity> target)
{
	if (target->isActive == false || target->GetComponent<Character>()->GetHealth() <= 0)
		return;

	target->GetComponent<Character>()->GetHit(damage);
	std::cout << "Player has been hit!" << std::endl;

	player = nullptr;
}

