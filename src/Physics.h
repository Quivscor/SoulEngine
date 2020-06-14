#pragma once
#include "Core.h"
#include "System.h"
#include "Transform.h"
#include "Collider.h"
#include <algorithm>

class Physics : public System
{
public:
	Physics();
	~Physics();
	void Init();
	virtual void Update() const;
	virtual void LateUpdate() const;
	void FixedUpdate() const;
	SystemType GetSystemType() const override { return SystemType::Physics; }

	//std::shared_ptr<Transform> transform;
	Transform* view;
	Transform* projection;

private:
	void MoveCollider(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const;
	bool CheckCollisions(std::shared_ptr<Collider> col1, std::shared_ptr<Collider> col2, std::shared_ptr<Transform> trns1, std::shared_ptr<Transform> trns2) const;
	bool TestFunction() const;
};