#pragma once
#include "GUI/Text.h"
#include "Core.h"
#include "ScriptableObject.h"
#include "Transform.h"

class WeaponComparator : public ScriptableObject
{

public:
	WeaponComparator();
	~WeaponComparator();

	void Show(bool value);

	void UpdateStats(float base, float oldValue, float newValue, std::shared_ptr<Entity> oldText, std::shared_ptr<Entity> newText);

	std::shared_ptr<Entity> damageTitle;
	std::shared_ptr<Entity> oldDamage;
	std::shared_ptr<Entity> damageCompare;
	std::shared_ptr<Entity> newDamage;

	std::shared_ptr<Entity> speedTitle;
	std::shared_ptr<Entity> oldASpeed;
	std::shared_ptr<Entity> speedCompare;
	std::shared_ptr<Entity> newASpeed;

	std::shared_ptr<Entity> durabilityTitle;
	std::shared_ptr<Entity> oldDurability;
	std::shared_ptr<Entity> durabilityCompare;
	std::shared_ptr<Entity> newDurability;
};