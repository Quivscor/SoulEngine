#pragma once
#include "Core.h"
#include "Component.h"

class ColliderElipse : public Component
{
public:
	ColliderElipse();
	ColliderElipse(glm::vec3 radius);
	~ColliderElipse();

	glm::vec3 radius;

	virtual ComponentType GetComponentType() const override { return ComponentType::ColliderElipseComponent; }

private:

};