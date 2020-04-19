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

	//optimalization required - dirty flag or sth
	for (int i = 0; i < m_Entities.size(); i++)
	{
		transform = m_Entities[i]->GetComponent<Transform>();

		if (transform->dirtyFlag == true)
		{
			transform->matrix = glm::mat4(1.0f);
			transform->matrix = glm::translate(transform->matrix, transform->position);
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
			transform->matrix = glm::scale(transform->matrix, transform->scale);

			transform->dirtyFlag = false;
		}
	}
}

void Physics::LateUpdate() const
{

}

void Physics::FixedUpdate() const
{
	std::shared_ptr<Transform> transform;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		transform = m_Entities[i]->GetComponent<Transform>();

		if (transform->moveVector == glm::vec3(0) && transform->rotateVector == glm::vec3(0))
			continue;
		
		transform->matrix = glm::translate(transform->matrix, transform->moveVector * (-1.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
		transform->matrix = glm::translate(transform->matrix, transform->moveVector * 2.0f);
		transform->moveVector = glm::vec3(0);
		transform->rotateVector = glm::vec3(0);
	}
}