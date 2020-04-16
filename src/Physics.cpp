#include "Physics.h"

Physics::Physics()
{

}

Physics::~Physics()
{

}

void Physics::Init() const
{ }

void Physics::Update() const
{
	std::shared_ptr<Transform> transform;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		transform = m_Entities[i]->GetComponent<Transform>();
		transform->matrix = glm::mat4(1.0f);
		transform->matrix = glm::translate(transform->matrix, transform->position);
		transform->matrix = glm::rotate(transform->matrix, (transform->rotation.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotation.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotation.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
		transform->matrix = glm::scale(transform->matrix, transform->scale);
	}
}

void Physics::LateUpdate() const
{

}