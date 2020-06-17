#pragma once
#include "Core.h"
#include "Component.h"
#include "Entity.h"
#include "Window.h"
#include "Frustum.h"

class Camera : public Component
{
public:
	friend class Renderer;

	Frustum m_Frustum;

	Camera();
	~Camera();

	void SetNearClippingPlane(float value);
	void SetFarClippingPlane(float value);
	void SetFieldOfView(float value);
	void SetLookAndUpVectors(std::shared_ptr<Entity> character);

	virtual ComponentType GetComponentType() const override { return ComponentType::CameraComponent; }

	glm::mat4 GetProjection();
	void CalculateFrustum();
	float DistanceFromCameraTarget(std::shared_ptr<Transform> transform);

	glm::vec3 upVector = glm::vec3(0.0f);
	glm::vec3 lookVector = glm::vec3(0.0f);
private:
	void UpdateProjection();

	glm::mat4 projection = glm::mat4(1.0f);

	float nearClippingPlane = 0.1f;
	float farClippingPlane = 1000.0f;
	float fieldOfView = 60.0f;
	std::shared_ptr<Transform> cameraTarget = nullptr;

	bool cameraVectorsSet = false;

};