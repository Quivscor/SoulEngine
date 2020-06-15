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

	CheckDeathCondition();
}

void Character::CheckDeathCondition()
{
	if (health <= 0)
	{
		if (thisEntity->layer == PlayerLayer)
		{
			EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->isActive = false;
			thisEntity->isActive = false;
			//playerReference->GetComponent<PlayerEnemyCommunicator>()->PlayerDead();
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