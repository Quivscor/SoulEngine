#include "WeaponFactory.h"

std::vector<WeaponModel> WeaponFactory::weapons;

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
	newWeapon.bonusOther = rand() % 50;

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
