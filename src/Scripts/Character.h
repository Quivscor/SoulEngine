#pragma once
#include "ScriptableObject.h"
#include "PlayerEnemyCommunicator.h"
#include "Billboard.h"
#include "TextRendering.h"

class Character : public ScriptableObject
{
public:
	Character();
	~Character();

	void GetHit(float damage);

	virtual ComponentType GetComponentType() const override { return ComponentType::CharacterComponent; }

	std::shared_ptr<Entity> playerReference = nullptr;

	float GetHealth();

	std::shared_ptr<Entity> gameLostText;

	std::shared_ptr<Billboard> healthBar;
	std::shared_ptr<HUD> HUDhealthBar;

	//debug
	std::shared_ptr<Text> healthDebugValue;

	void AddHealth(float value);

protected:
	//virtual void Start();


	void CheckDeathCondition();
	

	float maxHealth = 100.0f;
	float health = 100.0f;

};