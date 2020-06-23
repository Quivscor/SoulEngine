#include "WeaponFactory.h"

std::vector<WeaponModel> WeaponFactory::weapons;
std::shared_ptr<WeaponFactory>WeaponFactory::m_Instance = nullptr;
WeaponFactory::WeaponFactory()
{

}

WeaponFactory::~WeaponFactory()
{

}

void WeaponFactory::SetWeapon(Mesh* mesh, WeaponType type)
{
	WeaponModel newWeapon;
	newWeapon.model = mesh;
	newWeapon.type = type;

	weapons.push_back(newWeapon);
}

std::shared_ptr<WeaponStats> WeaponFactory::GetWeapon()
{
	if (weapons.size() == 0)
		return nullptr;

	WeaponStats newWeapon;
	int weaponModel = rand() % weapons.size();
	newWeapon.model = weapons[weaponModel];

	// axe
	if (weaponModel == 0)
	{
		newWeapon.bonusDamage = rand() % 20 + 5;
		newWeapon.bonusSpeed = ((rand() % 25) / 100.0f) + 2.0f;
		newWeapon.durability = rand() % 15 + 20;
		
	}
	// mace
	if (weaponModel == 1)
	{
		newWeapon.bonusDamage = rand() % 20;
		newWeapon.bonusSpeed = ((rand() % 50) / 100.0f) + 2.5f;
		newWeapon.durability = rand() % 15 + 15;

	}
	// sword
	if (weaponModel == 2)
	{
		newWeapon.bonusDamage = rand() % 15;
		newWeapon.bonusSpeed = ((rand() % 50) / 100.0f) + 2.0f;
		newWeapon.durability = rand() % 25 + 30;

	}

	newWeapon.maxDurability = newWeapon.durability;

	return std::make_shared<WeaponStats>(newWeapon);
}

std::shared_ptr<WeaponStats> WeaponFactory::GetDefaultWeapon()
{
	if (weapons.size() == 0)
		return nullptr;

	WeaponStats newWeapon;

	for(int i = 0; i < weapons.size(); i++)
		if (weapons[i].type == Sword)
			newWeapon.model = weapons[i];
	
	newWeapon.bonusDamage = 0;
	newWeapon.bonusSpeed = 2.0f;

	return std::make_shared<WeaponStats>(newWeapon);
}

/*std::shared_ptr<WeaponFactory> WeaponFactory::GetInstance()
{
	if (WeaponFactory::m_Instance == NULL)
	{
		WeaponFactory::m_Instance = std::make_shared<WeaponFactory>();
	}

	return WeaponFactory::m_Instance;
}*/
