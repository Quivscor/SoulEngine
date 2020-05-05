#pragma once
#include "Core.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();
	glm::vec3 GetLocalPosition();
	glm::vec3 GetLocalPositionFromMatrix();
	glm::vec3 GetLocalRotation();
	glm::vec3 GetLocalScale();
	glm::mat4 GetLocalMatrix();
	glm::vec3 GetGlobalPosition();
	glm::vec3 GetGlobalPositionFromMatrix();
	glm::vec3 GetGlobalRotation();
	glm::vec3 GetGlobalScale();
	glm::mat4 GetGlobalMatrix();
	void SetLocalPosition(glm::vec3 vector);
	void SetLocalRotation(glm::vec3 vector);
	void SetLocalScale(glm::vec3 vector);
	void SetLocalMatrix(glm::mat4 matrix);
	
	void SetParent(std::shared_ptr<Transform> parent);

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

	bool IsDirty();
	void SetDirty(bool value);

	glm::vec3 GetMoveVector();
	glm::vec3 GetRotateVector();
	void SetMoveVector(glm::vec3 vector);
	void SetRotateVector(glm::vec3 vector);

	void SetGlobalPosition();
	void SetGlobalRotation();
	void SetGlobalScale();
	void SetGlobalMatrix();

private:
	glm::mat4 matrix = glm::mat4(1);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 position = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 moveVector = glm::vec3(0);
	glm::vec3 rotateVector = glm::vec3(0);

	glm::mat4 m_GlobalMatrix;
	glm::vec3 m_GlobalPosition;
	glm::vec3 m_GlobalRotation;
	glm::vec3 m_GlobalScale;

	bool dirtyFlag = false;

	std::shared_ptr<Transform> m_Parent;
};