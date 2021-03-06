#pragma once
#include "Aura.h"

class AuraBonusDamage : public Aura
{
public:
	AuraBonusDamage() {};
	~AuraBonusDamage() {};

	virtual void LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter) override;
	virtual std::string ToString() override;

private:
	float bonusDamage = 5.0f;
};