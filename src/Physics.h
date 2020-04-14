#pragma once
#include "Core.h"
#include "System.h"
#include "Transform.h"

class Physics : System
{
public:
	Physics();
	~Physics();
	void Init() const;
	virtual void Update() const;
	virtual void LateUpdate() const;
	SystemType GetSystemType() const override { return SystemType::Physics; }

	Transform* transform;
	Transform* view;
	Transform* projection;

private:

};