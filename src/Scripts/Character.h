#pragma once
#include "ScriptableObject.h"
#include "PlayerEnemyCommunicator.h"
#include "Billboard.h"
#include "TextRendering.h"
#include "Model.h"

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
	std::shared_ptr<HUD> HUDsoulsBar;
	//std::shared_ptr<Particles> Parts;

	std::shared_ptr<Entity> aura1ref;
	std::shared_ptr<Entity> aura2ref;
	std::shared_ptr<Entity> aura3ref;

	//debug
	std::shared_ptr<Text> healthDebugValue;

	void AddHealth(float value);

	float maxHealth = 100.0f;
	float health = 100.0f;

	//for totem
	Model* usedTotem;

protected:
	//virtual void Start();


	void CheckDeathCondition();

	bool isDead = false;
	

};