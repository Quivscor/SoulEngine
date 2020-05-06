#pragma once
#include "Entity.h"
#include "Transform.h"

class Player : public Entity
{
public:
	Player();
	~Player();

	std::shared_ptr<Transform> m_Transform;
};

