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

	//kratosAxe->EnemyKilled();
}

void PlayerEnemyCommunicator::EnemySpawned()
{
	spawnedEnemies++;
}

