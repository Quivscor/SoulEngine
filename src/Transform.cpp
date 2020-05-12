#include "Core.h"
#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1, 1, 1);
	displayPositionInPhysicsCalculation = false;
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

bool Transform::IsDirty()
{
	return dirtyFlag;
}

void Transform::SetDirty(bool value)
{
	dirtyFlag = value;
}

glm::vec3 Transform::GetLocalPosition()
{
	return position;
}

glm::vec3 Transform::GetLocalPositionFromMatrix()
{
	return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
}

glm::vec3 Transform::GetLocalRotation()
{
	return rotation;
}

glm::vec3 Transform::GetLocalScale()
{
	return scale;
}

glm::mat4 Transform::GetLocalMatrix()
{
	return matrix;
}

glm::vec3 Transform::GetGlobalPosition()
{
	if (m_Parent != NULL)
	{
		glm::vec3 _position = m_Parent->GetGlobalPosition() + position;
		return _position;
	}		
	else
	{
		glm::vec3 _position = GetLocalPosition();
		return _position;
	}
		
}

glm::vec3 Transform::GetGlobalPositionFromMatrix()
{
	if (m_Parent != NULL)
		return m_Parent->GetGlobalPositionFromMatrix() + glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
	else
		return GetLocalPositionFromMatrix(); 
}

glm::vec3 Transform::GetGlobalRotation()
{
	if (m_Parent != NULL)
		return m_Parent->GetGlobalRotation() + rotation;
	else
		return GetLocalRotation();
}

glm::vec3 Transform::GetGlobalScale()
{
	if (m_Parent != NULL)
		return m_Parent->GetGlobalScale() + scale;
	else
		return GetLocalScale();
}

glm::mat4 Transform::GetGlobalMatrix()
{
	if (m_Parent != NULL)
		return m_Parent->GetGlobalMatrix() * matrix;
	else
		return GetLocalMatrix();
}

void Transform::SetLocalPosition(glm::vec3 vector)
{
	position = vector;
	dirtyFlag = true;

	Transform::SetGlobalPosition();
}

void Transform::SetLocalRotation(glm::vec3 vector)
{
	rotation = vector;
	dirtyFlag = true;

	Transform::SetGlobalRotation();
}

void Transform::SetLocalScale(glm::vec3 vector)
{
	scale = vector;
	dirtyFlag = true;

	Transform::SetGlobalScale();
}

void Transform::SetLocalMatrix(glm::mat4 matrix)
{
	this->matrix = matrix;
	dirtyFlag = true;

	Transform::SetGlobalMatrix();
}

void Transform::DisplayDebugInfo(bool value)
{
	this->displayPositionInPhysicsCalculation = value;
}

void Transform::ShowDebugInfo()
{
	if (this->displayPositionInPhysicsCalculation == true)
	{
		std::cout << "Global position: " << GetGlobalPositionFromMatrix().x << "x " << GetGlobalPositionFromMatrix().y << "y " << GetGlobalPositionFromMatrix().z << "z\n";
		std::cout << "Local position: " << GetLocalPositionFromMatrix().x << "x " << GetLocalPositionFromMatrix().y << "y " << GetLocalPositionFromMatrix().z << "z\n";
	}
}

void Transform::SetGlobalPosition()
{
	dirtyFlag = true;

	if (m_Parent != NULL)
		m_GlobalPosition = m_Parent->GetGlobalPosition() + position;
	else
		m_GlobalPosition = position;
}

void Transform::SetGlobalRotation()
{
	dirtyFlag = true;

	if (m_Parent != NULL)
		m_GlobalRotation = m_Parent->GetGlobalRotation() + rotation;
	else
		m_GlobalRotation = rotation;
}

void Transform::SetGlobalScale()
{
	dirtyFlag = true;

	if (m_Parent != NULL)
		m_GlobalScale = m_Parent->GetGlobalScale() + scale;
	else
		m_GlobalScale = scale;
}

void Transform::SetGlobalMatrix()
{
	dirtyFlag = true;

	if (m_Parent != NULL)
		m_GlobalMatrix = m_Parent->GetGlobalMatrix() * matrix;
	else
		m_GlobalMatrix = matrix;
}

void Transform::SetParent(std::shared_ptr<Transform> parent)
{
	m_Parent = parent;
}

glm::vec3 Transform::GetMoveVector()
{
	return moveVector;
}

glm::vec3 Transform::GetRotateVector()
{
	return rotateVector;
}

void Transform::SetMoveVector(glm::vec3 vector)
{
	moveVector = vector;
	dirtyFlag = true;
}

void Transform::SetRotateVector(glm::vec3 vector)
{
	rotateVector = vector;
	dirtyFlag = true;
}