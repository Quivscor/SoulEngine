#include "WeaponOnTheGround.h"

WeaponOnTheGround::WeaponOnTheGround() {}

WeaponOnTheGround::~WeaponOnTheGround() {}

void WeaponOnTheGround::Start() 
{
	if (thisEntity->GetComponent<Transform>() == nullptr)
	{
		thisEntity->AddComponent<Transform>();
		thisEntity->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 3.0f));
	}

	thisEntity->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

	std::vector<glm::vec2> colliderShape;
	colliderShape.push_back({ -8.0f, -5.0f });
	colliderShape.push_back({ -5.0f, -8.0f });
	colliderShape.push_back({ 5.0f, -8.0f });
	colliderShape.push_back({ 8.0f, -5.0f });
	colliderShape.push_back({ 8.0f, 5.0f });
	colliderShape.push_back({ 5.0f,  8.0f });
	colliderShape.push_back({ -5.0f, 8.0f });
	colliderShape.push_back({ -8.0f, 5.0f });

	thisEntity->AddComponent<Collider>();
	thisEntity->GetComponent<Collider>()->SetShape(colliderShape);

	thisEntity->GetComponent<Collider>()->isStatic = true;
	thisEntity->GetComponent<Collider>()->isTrigger = true;

	weapon = WeaponFactory::GetWeapon();

	if (weapon == nullptr)
		return;

	weapon->model;

	thisEntity->AddComponent<Mesh>();
	thisEntity->GetComponent<Mesh>()->indices = weapon->model.model->indices;
	thisEntity->GetComponent<Mesh>()->vertices = weapon->model.model->vertices;
	thisEntity->GetComponent<Mesh>()->material = weapon->model.model->material;
	thisEntity->GetComponent<Mesh>()->setupMesh();

}

void WeaponOnTheGround::Update() 
{
	thisEntity->GetComponent<Transform>()->Rotate(glm::vec3(0.0f, rotatingSpeed * Time::GetDeltaTime(), 0.0f));
}