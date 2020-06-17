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
	newWeapon.model = weapons[rand() % weapons.size()];
	newWeapon.bonusDamage = rand() % 50;
	newWeapon.bonusSpeed = ((rand() % 100) / 100.0f) + 2.0f;
	newWeapon.durability = rand() % 10 + 5;

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
