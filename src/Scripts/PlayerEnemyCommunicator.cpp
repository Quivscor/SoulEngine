#include "PlayerEnemyCommunicator.h"

PlayerEnemyCommunicator::PlayerEnemyCommunicator()
{

}

PlayerEnemyCommunicator::~PlayerEnemyCommunicator()
{

}

void PlayerEnemyCommunicator::EnemyDied()
{
	killedEnemiesCounter++;
	std::cout << "I'm dead" << std::endl;

	if (killedEnemiesCounter == spawnedEnemies)
	{
		gameWon->isActive = true;
		gameWon2->isActive = true;
	}
}

void PlayerEnemyCommunicator::EnemySpawned()
{
	spawnedEnemies++;
}

