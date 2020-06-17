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
		thisEntity->GetComponent<Transform>()->SetLocalPosition(objectToFollow->GetGlobalPosition() * glm::vec3(-1, 0, -1 / sqrt(2.0f)) + glm::vec3(0.0f, objectToFollow->GetGlobalPosition().z * (1 / sqrt(2.0f)), -10.0f));
	}
}

void CameraFollow::Update()
{
	if (objectToFollow != nullptr)
	{
		thisEntity->GetComponent<Transform>()->SetLocalPosition(objectToFollow->GetGlobalPosition() * glm::vec3(-1, 0, -1/sqrt(2.0f)) +  glm::vec3(0.0f, objectToFollow->GetGlobalPosition().z * (1 / sqrt(2.0f)), -10.0f));
	}
}