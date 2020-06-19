#include "Village.h"

Village::Village()
{

}

Village::~Village()
{

}

void Village::Start()
{
	std::vector<glm::vec2> colliderShape;

	colliderShape.push_back({ -5.0f, 7.0f });
	colliderShape.push_back({ 5.0f, 7.0f });
	colliderShape.push_back({ 7.0f, 5.0f });
	colliderShape.push_back({ 7.0f, -5.0f });
	colliderShape.push_back({ 5.0f, -7.0f });
	colliderShape.push_back({ -5.0f, -7.0f });
	colliderShape.push_back({ -7.0f, -5.0f });
	colliderShape.push_back({ -7.0f, 5.0f });

	thisEntity->AddComponent<Collider>();
	thisEntity->GetComponent<Collider>()->SetShape(colliderShape);
	thisEntity->GetComponent<Collider>()->isTrigger = true;
	thisEntity->GetComponent<Collider>()->isStatic = true;
}

void Village::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->layer == PlayerLayer)
	{
		std::cout << "Player in range" << std::endl;

		for (int i = 0; i < enemiesInVillage.size(); i++)
		{
			if (EntityManager::GetInstance()->GetEntity(enemiesInVillage[i]->GetOwnerID())->isActive == true)
			{
				enemiesInVillage[i]->playerPosition = EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->GetComponent<Transform>();
				enemiesInVillage[i]->isTriggered = true;
			}
		}
	}
}

void Village::OnTriggerExit(std::shared_ptr<Collider> other)
{
	if (EntityManager::GetInstance()->GetEntity(other->GetOwnerID())->layer == PlayerLayer)
	{
		std::cout << "Player out of range" << std::endl;
	}
}