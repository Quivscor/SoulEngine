#pragma once
#include "ScriptableObject.h"

class PlayerEnemyCommunicator : public ScriptableObject
{

public:
	PlayerEnemyCommunicator();
	~PlayerEnemyCommunicator();
	
	void EnemyDied();

	int killedEnemiesCounter = 0;

	int spawnedEnemies = 0;

	void EnemySpawned();
};