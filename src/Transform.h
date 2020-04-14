#pragma once
#include "Core.h"
#include "Component.h"

class Transform : public Component
{
public:
	friend class Physics;
	friend class Renderer;

	Transform();
	~Transform();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	virtual ComponentType GetComponentType() const override { return ComponentType::TransformComponent; }

private:
	glm::mat4 matrix = glm::mat4(1);
};