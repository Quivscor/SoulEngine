#pragma once
#include "Core.h"
#include "System.h"
#include "Transform.h"
#include "CollisionDetector.h"

class CollisionDetector;

class Physics : public System
{
public:
	Physics();
	~Physics();
	void Init() const;
	virtual void Update() const;
	virtual void LateUpdate() const;
	void FixedUpdate() const;
	SystemType GetSystemType() const override { return SystemType::Physics; }

	//std::shared_ptr<Transform> transform;
	Transform* view;
	Transform* projection;

private:
	CollisionDetector* collisionDetector = nullptr;
};