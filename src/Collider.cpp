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

			std::cout << "On Trigger stay!" << std::endl;
			//execute OnTriggerStay(collision[i])

			return;
		}
	}

	Collision newCollision;
	newCollision.collider = otherObjCollider;
	newCollision.controlFlag = controlFlag;

	collisions.push_back(newCollision);

	std::cout << "On Trigger enter!" << std::endl;
	//execute OnTriggerEnter(newCollision)
}

void Collider::CheckControlFlags()
{
	auto it = collisions.begin();

	while (it != collisions.end())
	{
		if (it->controlFlag != controlFlag)
		{
			std::cout << "On Trigger exit!" << std::endl;
			//execute OnTriggerExit(it->collider)
			it = collisions.erase(it);
		}
		else
			++it;
	}
}