#pragma once
#include "Entity.h"
#include "Transform.h"

class Player : public Entity
{
public:
	Player(ComponentManager* m_ComponentManager);
	~Player();

	std::shared_ptr<Transform> m_Transform;
};

