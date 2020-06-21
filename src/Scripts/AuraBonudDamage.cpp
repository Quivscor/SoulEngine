#include "AuraBonusDamage.h"

void AuraBonusDamage::LoadAura(std::shared_ptr<Enemy> enemyScr, std::shared_ptr<Character> enemyCharacter)
{
	enemyScr->weapon->GetComponent<EnemyWeapon>()->AddBasicDmg(bonusDamage);
}

std::string AuraBonusDamage::ToString()
{
	return "AuraBonusDamage";
}
