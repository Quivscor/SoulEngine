#pragma once
#include "Aura.h"

class AuraRunningSpeed : public Aura
{
public:
	AuraRunningSpeed() {};
	~AuraRunningSpeed() {};

	virtual void LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter) override;
	virtual std::string ToString() override;

private:
	float bonusRunningSpeed = 0.5f;
};