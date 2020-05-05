#include "Collider.h"

Collider::Collider()
{

}

Collider::~Collider()
{

}

void Collider::SetShape(std::vector<glm::vec2> shape)
{
	this->polygon.shape = std::vector<glm::vec2>(shape);
	this->polygon.points = std::vector<glm::vec2>(shape);
}

void Collider::IncreaseControlFlag()
{
	controlFlag++;
}

void Collider::AddTriggerCollision(std::shared_ptr<Collider> otherObjCollider)
{
	for (int i = 0; i < collisions.size(); i++)
	{
		if (collisions[i].collider == otherObjCollider)
		{
			collisions[i].controlFlag = controlFlag;

			//execute OnTriggerStay(collision[i])
			EntityManager::GetInstance()->GetEntity(this->GetOwnerID())->GetComponent<ScriptableObject>()->OnTriggerStay(otherObjCollider);

			return;
		}
	}

	Collision newCollision;
	newCollision.collider = otherObjCollider;
	newCollision.controlFlag = controlFlag;

	collisions.push_back(newCollision);

	//execute OnTriggerEnter(newCollision)
	EntityManager::GetInstance()->GetEntity(this->GetOwnerID())->GetComponent<ScriptableObject>()->OnTriggerEnter(otherObjCollider);
}

void Collider::CheckControlFlags()
{
	auto it = collisions.begin();

	while (it != collisions.end())
	{
		if (it->controlFlag != controlFlag)
		{
			//execute OnTriggerExit(it->collider)
			EntityManager::GetInstance()->GetEntity(this->GetOwnerID())->GetComponent<ScriptableObject>()->OnTriggerExit(it->collider);
			it = collisions.erase(it);
		}
		else
			++it;
	}
}