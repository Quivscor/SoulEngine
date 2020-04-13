#pragma once
#include "Core.h"
#include "System.h"
#include "Transform.h"

class Physics : System
{
public:
	Physics(int ID);
	~Physics();
	virtual void Update() const;
	virtual void LateUpdate() const;

	Transform* transform;
	Transform* view;
	Transform* projection;

private:

};