#include "AuraRunningSpeed.h"

void AuraRunningSpeed::LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter)
{
	enemyScr->runningSpeed += bonusRunningSpeed;
}

std::string AuraRunningSpeed::ToString()
{
	return "AuraRunningSpeed";
}
