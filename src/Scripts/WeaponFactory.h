#pragma once
#include "../Mesh.h"
#include <time.h>

enum WeaponType
{
	Sword,
	Mace,
	Axe
};

struct WeaponModel
{
	WeaponType type;
	Mesh* model;
};

struct WeaponStats
{
	WeaponModel model;
	float bonusDamage;
	float bonusOther;
};

static class WeaponFactory
{
public:
	WeaponFactory();
	~WeaponFactory();

	//static std::shared_ptr<WeaponFactory> GetInstance();

	static void SetWeapon(Mesh* mesh, WeaponType type);

	static std::shared_ptr<WeaponStats> GetWeapon();
	static std::shared_ptr<WeaponStats> GetDefaultWeapon();

private:
	static std::vector<WeaponModel> weapons;
	static std::shared_ptr<WeaponFactory> m_Instance;
};