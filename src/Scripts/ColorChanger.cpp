#include "ColorChanger.h"

ColorChanger::ColorChanger()
{

}

ColorChanger::~ColorChanger()
{

}

void ColorChanger::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Mesh>() == nullptr)
		return;

	Material* mat = EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Mesh>()->material;
	if (mat != nullptr)
	{
		mat->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void ColorChanger::OnTriggerStay(std::shared_ptr<Collider> other)
{

}

void ColorChanger::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Mesh>() == nullptr)
		return;

	Material* mat = EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Mesh>()->material;
	if (mat != nullptr)
	{
		mat->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}