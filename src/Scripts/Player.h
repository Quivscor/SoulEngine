#pragma once
#include "ScriptableObject.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Model.h"
#include "WeaponComparator.h"
#include "PlayerEnemyCommunicator.h"

enum AnimationType
{
	PlayerAnimationIdle,
	PlayerAnimationRun,
	PlayerAnimationAttack,
	PlayerAnimationRoll,
	PlayerAnimationDeath
};

class PlayerEnemyCommunicator;

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

	//no weapon
	Model* animationIdle;
	Model* animationRun;
	Model* animationAttack;
	Model* animationRoll;

	Model* animationDeath;

	Renderer* renderer;

	std::shared_ptr<WeaponComparator> weaponComparator;
	std::shared_ptr<Entity> berserkerModeText;
	std::shared_ptr<PlayerEnemyCommunicator> pec;

	void EnemyKilled();
	std::shared_ptr<HUD> HUDDurability;
	std::shared_ptr<Collider> characterCollider;

protected:
	virtual void Start();
	virtual void Update();

	virtual void OnTriggerEnter(std::shared_ptr<Collider> other) override;
	virtual void OnTriggerExit(std::shared_ptr<Collider> other) override;

private:
	void CreateWeapon();
	void Swap();
	void Move();
	void CheckWeapon();
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

	bool berserkerModeActive = false;
	float berserkerModeDuration = 10.0f;
	float berserkerCurrentTime = 0.0f;
	int killedEnemies = 0;
	int enemiesRequiredToStartBerserkerMode = 3;

	void RunBerserkerMode();
	void DisableBerserkerMode();

	bool killstreakCounterActive = false;
	float killstreakTimer = 10.0f;
	float killstreakCurrentTime = 0.0f;

	int lastEnemyCounter = 0;
};