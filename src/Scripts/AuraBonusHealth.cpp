#include "AuraBonusHealth.h"

void AuraBonusHealth::LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter)
{
	enemyCharacter->AddHealth(bonusHealth);
}