#include "Camera.h"

Camera::Camera()
{
	projection = glm::perspective(glm::radians(fieldOfView), 
		(float)Window::GetInstance()->m_Width / (float)Window::GetInstance()->m_Height, 
		nearClippingPlane, farClippingPlane);
}

Camera::~Camera()
{

}

void Camera::SetNearClippingPlane(float value)
{
	nearClippingPlane = value;
}

void Camera::SetFarClippingPlane(float value)
{
	farClippingPlane = value;
}

void Camera::SetFieldOfView(float value)
{
	fieldOfView = value;
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}