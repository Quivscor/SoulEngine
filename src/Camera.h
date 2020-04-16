#pragma once
#include "Core.h"
#include "Component.h"
#include "Window.h"

class Camera : public Component
{
public:
	friend class Renderer;

	Camera();
	~Camera();

	void SetNearClippingPlane(float value);
	void SetFarClippingPlane(float value);
	void SetFieldOfView(float value);

	virtual ComponentType GetComponentType() const override { return ComponentType::CameraComponent; }

	glm::mat4 GetProjection();

private:
	void UpdateProjection();

	glm::mat4 projection;

	float nearClippingPlane = 0.1f;
	float farClippingPlane = 1000.0f;
	float fieldOfView = 60.0f;
};