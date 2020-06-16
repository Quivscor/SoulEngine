#pragma once
#ifndef PLAYERENEMYCOMMUNICATOR
#define PLAYERENEMYCOMMUNICATOR
#include "ScriptableObject.h"
//#include "Character.h"
//#include "KratosAxe.h"

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

	std::shared_ptr<Entity> gameWon;
	std::shared_ptr<Entity> gameWon2;

	//std::shared_ptr<KratosAxe> kratosAxe;
};

#endif