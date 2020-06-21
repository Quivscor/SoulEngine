#include "Character.h"

Character::Character()
{

}

Character::~Character()
{
	
}

/*void Character::Start()
{
	//health = maxHealth;
}*/

void Character::GetHit(float damage)
{
	std::cout << "Getting damage: " << damage << std::endl;

	health -= damage;

	if (thisEntity->layer == PlayerLayer)
	{
		healthDebugValue->text = std::to_string((int)glm::floor(health)) + "%";
	}

	CheckDeathCondition();

	//thisEntity->GetComponent<Billboard>()->setLife(health);
	healthBar->setLife(health);
	if(HUDhealthBar!=nullptr)
	HUDhealthBar->setLife(health);
}

void Character::CheckDeathCondition()
{
	if (health <= 0)
	{
		if (thisEntity->layer == PlayerLayer)
		{
			gameLostText->isActive = true;
			//playerReference->GetComponent<PlayerEnemyCommunicator>()->PlayerDead();
			EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->isActive = false;
			thisEntity->isActive = false;
		}
		else
		{
			thisEntity->isActive = false;

			if (playerReference->GetComponent<PlayerEnemyCommunicator>() != nullptr)
				playerReference->GetComponent<PlayerEnemyCommunicator>()->EnemyDied();
		}
	}
}

float Character::GetHealth()
{
	return health;
}

void Character::AddHealth(float value)
{
	health += value;

	if (thisEntity->layer == PlayerLayer)
	{
		healthDebugValue->text = std::to_string((int)glm::floor(health)) + "%";
	}

	//thisEntity->GetComponent<Billboard>()->setLife(health);
	healthBar->setLife(health);
	if (HUDhealthBar != nullptr)
		HUDhealthBar->setLife(health);
}