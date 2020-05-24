#pragma once
#include "ScriptableObject.h"
#include "InputHandler.h"
#include "Weapon.h"
#include "Model.h"

enum AnimationType
{
	PlayerAnimationIdle,
	PlayerAnimationRun,
	PlayerAnimationAttack
};

class Player : public ScriptableObject
{
public:
	Player();
	~Player();

	std::shared_ptr<Entity> inputHandler;

	std::shared_ptr<Weapon> weapon;

	Shader* shader;

	Model* animationIdle;
	Model* animationRun;
	Model* animationAttack;

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
	float attackTime = 1.0f;
	float currentAttackTime = 0.0f;

	short movingFB = 0; //moving forward/backward
	short movingLR = 0; //moving left/right

	Model* currentAnimation;
};