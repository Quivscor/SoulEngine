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
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	void SetPosition(glm::vec3 vector);
	void SetRotation(glm::vec3 vector);
	void SetScale(glm::vec3 vector);

	virtual ComponentType GetComponentType() const override { return ComponentType::TransformComponent; }

	static glm::vec3 Forward() { return glm::vec3(0, 0, 1); }
	static glm::vec3 Back() { return glm::vec3(0, 0, -1); }
	static glm::vec3 Down() { return glm::vec3(0, -1, 0); }
	static glm::vec3 Up() { return glm::vec3(0, 1, 0); }
	static glm::vec3 Left() { return glm::vec3(-1, 0, 0); }
	static glm::vec3 Right() { return glm::vec3(1, 0, 0); }

	//fixed update
	void Move(glm::vec3 vector);
	void Rotate(glm::vec3 vector);

private:
	glm::mat4 matrix = glm::mat4(1);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 moveVector = glm::vec3(0);
	glm::vec3 rotateVector = glm::vec3(0);

	bool dirtyFlag = false;
};