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
}

void PlayerEnemyCommunicator::EnemySpawned()
{
	spawnedEnemies++;
}

