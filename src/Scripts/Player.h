#pragma once
#include "ScriptableObject.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Model.h"
#include "WeaponComparator.h"

enum AnimationType
{
	PlayerAnimationIdle,
	PlayerAnimationRun,
	PlayerAnimationAttack,
	PlayerAnimationRoll,
	PlayerAnimationDeath
};

class Player : public ScriptableObject
{
public:
	Player();
	~Player();

	std::shared_ptr<Entity> inputHandler;

	std::shared_ptr<Weapon> weapon;

	Shader* shader;

	//sword
	Model* animationSwordIdle;
	Model* animationSwordRun;
	Model* animationSwordAttack;
	Model* animationSwordRoll;

	//axe
	Model* animationAxeIdle;
	Model* animationAxeRun;
	Model* animationAxeAttack;
	Model* animationAxeRoll;

	//mace
	Model* animationMaceIdle;
	Model* animationMaceRun;
	Model* animationMaceAttack;
	Model* animationMaceRoll;

	Model* animationDeath;

	std::shared_ptr<WeaponComparator> weaponComparator;

protected:
	virtual void Start();
	virtual void Update();

	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;

private:
	void CreateWeapon();
	void Swap();
	void Move();
	void CalculateRotation();
	void ChangeAnimation(AnimationType type);

	std::shared_ptr<WeaponOnTheGround> weaponInRange;
	float rotatingSpeed = 5.0f;
	bool isMoving = false;
	bool canMove = true;

	//weapon
	bool isAttacking = false;
	bool isRolling = false;

	float attackTime = 0.5f;
	float currentAttackTime = 0.0f;

	float rollTime = 1.0f;
	float currentRollTime = 0.0f;
	bool canRoll = true;

	short movingFB = 0; //moving forward/backward
	short movingLR = 0; //moving left/right

	float attackSpeed = 2.0f;

	Model* currentAnimation;
};