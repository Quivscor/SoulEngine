#pragma once
#include "ScriptableObject.h"
#include "WeaponOnTheGround.h"
#include "Enemy.h"
#include "AudioMaster.h"
#include "Source.h"

class Weapon : public ScriptableObject
{

public:
	Weapon();
	~Weapon();

	void Use();
	void TurnOffCollider();

	std::shared_ptr<Entity> characterContainer;

	std::shared_ptr<WeaponStats> GetWeapon();
	void SetWeapon(std::shared_ptr<WeaponStats> newWeapon);
	AudioMaster *audioMaster;
	Source source;

protected:
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnTriggerStay(std::shared_ptr<Collider> other) override;

private:
	std::shared_ptr<WeaponStats> equipedWeapon;

	float damage = 25.0f;
	std::vector<std::shared_ptr<Entity>> hitObjects;
};