#include "CameraFollow.h"

CameraFollow::CameraFollow()
{

}

CameraFollow::~CameraFollow()
{

}

void CameraFollow::Start()
{
	thisEntity->AddComponent<Transform>();
	thisEntity->AddComponent<Camera>();

	if (objectToFollow != nullptr)
	{
		thisEntity->GetComponent<Transform>()->SetPosition(objectToFollow->GetPositionFromMatrix() * glm::vec3(-1.0f, -1.0f, -1.0f) + glm::vec3(0.0f, -1.0f, -2.0f));
	}
}

void CameraFollow::Update()
{
	if (objectToFollow != nullptr)
	{
		thisEntity->GetComponent<Transform>()->SetPosition(objectToFollow->GetPositionFromMatrix() * glm::vec3(-1.0f, -1.0f, -1.0f) + glm::vec3(0.0f, -1.0f, -2.0f));
	}
}