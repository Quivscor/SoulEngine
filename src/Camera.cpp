#include "Camera.h"

Camera::Camera()
{
	UpdateProjection();
}

Camera::~Camera()
{

}

void Camera::SetNearClippingPlane(float value)
{
	nearClippingPlane = value;
	UpdateProjection();
}

void Camera::SetFarClippingPlane(float value)
{
	farClippingPlane = value;
	UpdateProjection();
}

void Camera::SetFieldOfView(float value)
{
	fieldOfView = value;
	UpdateProjection();
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}

void Camera::UpdateProjection()
{
	projection = glm::perspective(glm::radians(fieldOfView),
		(float)Window::GetInstance()->m_Width / (float)Window::GetInstance()->m_Height,
		nearClippingPlane, farClippingPlane);
}