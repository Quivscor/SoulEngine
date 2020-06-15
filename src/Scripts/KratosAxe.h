#pragma once
#ifndef KRATOSAXE
#define KRATOSAXE
#include "ScriptableObject.h"
#include "Character.h"
#include "PlayerEnemyCommunicator.h"
#include "TextRendering.h"

class KratosAxe : public ScriptableObject
{

public:
	KratosAxe();
	~KratosAxe();

	std::shared_ptr<Character> playersCharacter;
	std::shared_ptr<PlayerEnemyCommunicator> pec;

	void EnemyKilled();

	std::shared_ptr<Text> debugSoulsTaken;
	std::shared_ptr<Text> debugAxeStatus;

protected:
	virtual void Start() override;
	virtual void Update() override;

	bool axeIsHungry = false;

	float maxAxeSoulLevel = 100.0f;
	float currentAxeSoulLevel = 0.0f;
	float soulTakingInTimeModificator = 2.0f;
	float soulBoostPerExecution = 20.0f;
	
	float hpLostFromAxe = 10.0f;
	float currentTimeToTakeDmg = 0.0f;
	float timeBetweenDmgFromAxe = 10.0f;

	int enemiesOnMap = -500;
	int currentEnemiesOnMap = -500;
};

#endif