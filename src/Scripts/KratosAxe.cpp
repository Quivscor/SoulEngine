#include "KratosAxe.h"

KratosAxe::KratosAxe()
{

}

KratosAxe::~KratosAxe()
{

}

void KratosAxe::Start()
{
	currentAxeSoulLevel = maxAxeSoulLevel;
}

void KratosAxe::Update()
{
	enemiesOnMap = pec->spawnedEnemies - pec->killedEnemiesCounter;

	//std::cout << currentEnemiesOnMap << " : " << enemiesOnMap << std::endl;

	if (currentEnemiesOnMap == -500)
		currentEnemiesOnMap = enemiesOnMap;

	if (currentEnemiesOnMap != enemiesOnMap)
	{
		for (int i = 0; i < currentEnemiesOnMap - enemiesOnMap; i++)
			EnemyKilled();

		currentEnemiesOnMap = enemiesOnMap;
	}

	//std::cout << currentAxeSoulLevel << std::endl;

	debugSoulsTaken->text = std::to_string(pec->spawnedEnemies - currentEnemiesOnMap) + "";

	debugAxeStatus->text = std::to_string((int)glm::floor(currentAxeSoulLevel)) + "%";

	axeIsHungry = currentAxeSoulLevel <= 0.0f;

	if (axeIsHungry)
	{
		currentTimeToTakeDmg += TimeCustom::GetDeltaTime();

		if (currentTimeToTakeDmg >= timeBetweenDmgFromAxe)
		{
			if (EntityManager::GetInstance()->GetEntity(playersCharacter->GetOwnerID())->isActive == true)
			{
				playersCharacter->GetHit(hpLostFromAxe);
			}

			currentTimeToTakeDmg = 0.0f;
		}
	}
	else
	{
		currentTimeToTakeDmg = 0.0f;

		currentAxeSoulLevel -= TimeCustom::GetDeltaTime() * soulTakingInTimeModificator;

		if (currentAxeSoulLevel < 0.0f)
			currentAxeSoulLevel = 0.0f;
	}
}

void KratosAxe::EnemyKilled()
{
	currentAxeSoulLevel += soulBoostPerExecution;

	if (currentAxeSoulLevel > 100.0f)
		currentAxeSoulLevel = 100.0f;

	std::cout << currentAxeSoulLevel << std::endl;
}