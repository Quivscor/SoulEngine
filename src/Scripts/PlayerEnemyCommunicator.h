#pragma once
#ifndef PLAYERENEMYCOMMUNICATOR
#define PLAYERENEMYCOMMUNICATOR
#include "ScriptableObject.h"
//#include "Player.h"

//class Player;

class PlayerEnemyCommunicator : public ScriptableObject
{

public:
	PlayerEnemyCommunicator();
	~PlayerEnemyCommunicator();
	
	void EnemyDied();

	int killedEnemiesCounter = 0;

	int spawnedEnemies = 0;

	void EnemySpawned();

	void PlayerDead();

	//Player* playerReference;
};

#endif