#pragma once
#include "ScriptableObject.h"

class Character : public ScriptableObject
{
public:
	Character();
	~Character();

	void GetHit(float damage);

	virtual ComponentType GetComponentType() const override { return ComponentType::CharacterComponent; }

protected:
	//virtual void Start();


	void CheckDeathCondition();
	

	float maxHealth = 100.0f;
	float health = 100.0f;

};