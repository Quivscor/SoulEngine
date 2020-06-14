#pragma once
#include "ScriptableObject.h"
#include "PlayerEnemyCommunicator.h"

class GameManager : public ScriptableObject
{

public:
	GameManager();
	~GameManager();

	std::shared_ptr<PlayerEnemyCommunicator> pec;

	virtual void Update() override;
};