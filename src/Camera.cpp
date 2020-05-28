#include "Camera.h"
#include "Transform.h"
#include "EntityManager.h"

#define thisEntity EntityManager::GetInstance()->GetEntity(GetOwnerID())

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

void Camera::SetLookAndUpVectors(std::shared_ptr<Entity> character)
{ 
	cameraTarget = character->GetComponent<Transform>();;
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

void Camera::CalculateFrustum() {
	if (cameraVectorsSet == false)
	{
		std::shared_ptr<Transform> thistransform = thisEntity->GetComponent<Transform>();
		glm::vec3 thisPosition = thistransform->GetGlobalPosition();
		/*lookVector = cameraTarget->GetGlobalPosition() - thisPosition;*/
		lookVector = glm::vec3(0, 1 / sqrt(2.0f), -1 / sqrt(2.0f));
		upVector = glm::normalize(glm::cross(lookVector, Transform::Right()));
		lookVector = glm::normalize(lookVector);
		lookVector = glm::vec3(lookVector.x, lookVector.y, lookVector.z);

		cameraVectorsSet = true;
	}

	Frustum returnFrustum;
	float fnear = nearClippingPlane;
	float ffar = farClippingPlane;

	float aspect = (float)Window::GetInstance()->m_Width / (float)Window::GetInstance()->m_Height;
	std::shared_ptr<Transform> transform = thisEntity->GetComponent<Transform>();

	// Calculate the near and far plane points
	float nearHeight = 2 * tan((fieldOfView * 3.14f/180.0f) / 2) * fnear;
	float nearWidth = nearHeight * aspect;
	float farHeight = 2 * tan((fieldOfView * 3.14f / 180.0f) / 2) * ffar;
	float farWidth = farHeight * aspect;

	// And their centers
	glm::vec3 cameraPos = transform->GetGlobalPosition();
	glm::vec3 nearCenter = cameraPos/* + (lookVector * fnear)*/;
	glm::vec3 farCenter = cameraPos + (lookVector * ffar);

	returnFrustum.center = transform->GetGlobalPosition() + (lookVector * (ffar / 2.0f));

	// And the actual coordinates
	returnFrustum.fnear[Frustum::COORD_BOTTOMLEFT] = nearCenter - (upVector * (nearHeight / 2.0f)) - (Transform::Right() * (nearWidth / 2.0f));
	returnFrustum.fnear[Frustum::COORD_BOTTOMRIGHT] = nearCenter - (upVector * (nearHeight / 2.0f)) + (Transform::Right() * (nearWidth / 2.0f));
	returnFrustum.fnear[Frustum::COORD_TOPLEFT] = nearCenter + (upVector * (nearHeight / 2.0f)) - (Transform::Right() * (nearWidth / 2.0f));
	returnFrustum.fnear[Frustum::COORD_TOPRIGHT] = nearCenter + (upVector * (nearHeight / 2.0f)) + (Transform::Right() * (nearWidth / 2.0f));

	returnFrustum.ffar[Frustum::COORD_BOTTOMLEFT] = farCenter - (upVector * (farHeight / 2.0f)) - (Transform::Right() * (farWidth / 2.0f));
	returnFrustum.ffar[Frustum::COORD_BOTTOMRIGHT] = farCenter - (upVector * (farHeight / 2.0f)) + (Transform::Right() * (farWidth / 2.0f));
	returnFrustum.ffar[Frustum::COORD_TOPLEFT] = farCenter + (upVector * (farHeight / 2.0f)) - (Transform::Right() * (farWidth / 2.0f));
	returnFrustum.ffar[Frustum::COORD_TOPRIGHT] = farCenter + (upVector * (farHeight / 2.0f)) + (Transform::Right() * (farWidth / 2.0f));

	// Then create our frustum planes
	returnFrustum.planes[Frustum::PLANE_NEAR].CreateFromPoints(m_Frustum.fnear[Frustum::COORD_BOTTOMLEFT], m_Frustum.fnear[Frustum::COORD_BOTTOMRIGHT], m_Frustum.fnear[Frustum::COORD_TOPLEFT]);
	returnFrustum.planes[Frustum::PLANE_FAR].CreateFromPoints(m_Frustum.ffar[Frustum::COORD_TOPLEFT], m_Frustum.ffar[Frustum::COORD_TOPRIGHT], m_Frustum.ffar[Frustum::COORD_BOTTOMLEFT]);
	returnFrustum.planes[Frustum::PLANE_LEFT].CreateFromPoints(m_Frustum.fnear[Frustum::COORD_BOTTOMLEFT], m_Frustum.fnear[Frustum::COORD_TOPLEFT], m_Frustum.ffar[Frustum::COORD_BOTTOMLEFT]);
	returnFrustum.planes[Frustum::PLANE_RIGHT].CreateFromPoints(m_Frustum.ffar[Frustum::COORD_TOPRIGHT], m_Frustum.fnear[Frustum::COORD_TOPRIGHT], m_Frustum.ffar[Frustum::COORD_BOTTOMRIGHT]);
	returnFrustum.planes[Frustum::PLANE_TOP].CreateFromPoints(m_Frustum.ffar[Frustum::COORD_TOPLEFT], m_Frustum.ffar[Frustum::COORD_TOPRIGHT], m_Frustum.fnear[Frustum::COORD_TOPLEFT]);
	returnFrustum.planes[Frustum::PLANE_BOTTOM].CreateFromPoints(m_Frustum.fnear[Frustum::COORD_BOTTOMLEFT], m_Frustum.ffar[Frustum::COORD_BOTTOMLEFT], m_Frustum.fnear[Frustum::COORD_BOTTOMRIGHT]);

	m_Frustum = returnFrustum;
}

float Camera::DistanceFromCameraTarget(std::shared_ptr<Transform> transform)
{
	glm::vec3 position = transform->GetGlobalPosition();
	glm::vec3 cameraTarget = this->cameraTarget->GetGlobalPosition();
	return sqrt((cameraTarget.x - position.x) * (cameraTarget.x - position.x) + ((cameraTarget.z - 7.5f) - position.z) * ((cameraTarget.z - 7.5f) - position.z));
}