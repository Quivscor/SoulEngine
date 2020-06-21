#pragma once
#include "Aura.h"

class AuraBonusHealth : public Aura
{
public:
	AuraBonusHealth() {};
	~AuraBonusHealth() {};

	virtual void LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter) override;
	virtual std::string ToString() override;

private:
	float bonusHealth = 50.0f;
};