#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1, 1, 1);
}

Transform::~Transform()
{

}

void Transform::Move(glm::vec3 vector)
{
	moveVector += vector;
}

void Transform::Rotate(glm::vec3 vector)
{
	rotateVector += vector;
}

glm::vec3 Transform::GetPosition()
{
	return position;
}

glm::vec3 Transform::GetRotation()
{
	return rotation;
}

glm::vec3 Transform::GetScale()
{
	return scale;
}

void Transform::SetPosition(glm::vec3 vector)
{
	position = vector;
	dirtyFlag = true;
}

void Transform::SetRotation(glm::vec3 vector)
{
	rotation = vector;
	dirtyFlag = true;
}

void Transform::SetScale(glm::vec3 vector)
{
	scale = vector;
	dirtyFlag = true;
}