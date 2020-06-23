#include "Character.h"

Character::Character()
{
	 maxHealth = 100.0f;
	
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

	std::cout << health/maxHealth <<std::endl;
	//thisEntity->GetComponent<Billboard>()->setLife(health);
	if (healthBar != nullptr)
		healthBar->setLife(health/ maxHealth);
	if(HUDhealthBar!=nullptr)
		HUDhealthBar->setLife(health / maxHealth);
}

void Character::CheckDeathCondition()
{
	if (health <= 0)
	{
		if (isDead)
			return;

		isDead = true;

		if (thisEntity->layer == PlayerLayer)
		{
			gameLostText->isActive = true;
			//playerReference->GetComponent<PlayerEnemyCommunicator>()->PlayerDead();
			EntityManager::GetInstance()->GetEntity(thisEntity->GetComponent<Transform>()->GetParent()->GetOwnerID())->isActive = false;
			thisEntity->isActive = false;
		}
		else if(thisEntity->layer == EnemyLayer)
		{
			thisEntity->isActive = false;

			if (aura1ref != nullptr)
				aura1ref->isActive = false;
			if (aura2ref != nullptr)
				aura2ref->isActive = false;
			if (aura3ref != nullptr)
				aura3ref->isActive = false;

			if (playerReference->GetComponent<PlayerEnemyCommunicator>() != nullptr)
				playerReference->GetComponent<PlayerEnemyCommunicator>()->EnemyDied();
		}
		else if (thisEntity->layer == TotemLayer)
		{
			//thisEntity->isActive = false;
			
			thisEntity->GetComponent<Mesh>()->indices = usedTotem->GetMeshes()[0].indices;
			thisEntity->GetComponent<Mesh>()->vertices = usedTotem->GetMeshes()[0].vertices;
			thisEntity->GetComponent<Mesh>()->material = usedTotem->GetMeshes()[0].material;
			thisEntity->GetComponent<Mesh>()->setupMesh();

			if (playerReference->GetComponent<PlayerEnemyCommunicator>() != nullptr)
				playerReference->GetComponent<PlayerEnemyCommunicator>()->TotemDied();
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
	this->maxHealth += value;
	if (thisEntity->layer == PlayerLayer)
	{
		healthDebugValue->text = std::to_string((int)glm::floor(health)) + "%";
	}

	//thisEntity->GetComponent<Billboard>()->setLife(health);
	/*healthBar->setLife(health/ maxHealth);
	if (HUDhealthBar != nullptr)
		HUDhealthBar->setLife(health / maxHealth);*/
}