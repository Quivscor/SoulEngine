#include "Physics.h"

Physics::Physics()
{
	collisionDetector = new CollisionDetector();
}

Physics::~Physics()
{

}

void Physics::Init() const
{ 
	
}

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

		//if (i != 0)
		//	continue;
		
		transform->matrix = glm::translate(transform->matrix, transform->moveVector * (-1.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
		transform->matrix = glm::translate(transform->matrix, transform->moveVector);

		collisionDetector->Initialize(transform->GetPositionFromMatrix(), transform->moveVector, glm::vec3(-2, -500, -500), glm::vec3(-2, -500, 500), glm::vec3(-2, 1000, 0), 1.0f, 1.0f, 1.0f);
		
		//bool collision = collisionDetector->DetectCollision();

		//if (collision)
		//{
		//	std::cout << "Collision with wall" << std::endl;
		//	transform->matrix = glm::translate(transform->matrix, transform->moveVector * (-0.5f));
		//}
		//else
			transform->matrix = glm::translate(transform->matrix, transform->moveVector);

		transform->moveVector = glm::vec3(0);
		transform->rotateVector = glm::vec3(0);
	}

	/*std::cout << "Test 1" << std::endl;
	collisionDetector->Initialize(glm::vec3(0), glm::vec3(0, 0, 5), glm::vec3(-500, 500, 5), glm::vec3(500, 500, 5), glm::vec3(0, -10000, 5), 1, 1, 1);
	std::cout << collisionDetector->DetectCollision() << std::endl;
	std::cout << "Test 2" << std::endl;
	collisionDetector->Initialize(glm::vec3(0), glm::vec3(0, 0, 5), glm::vec3(0, 0, 2), glm::vec3(0, 6, 5), glm::vec3(0, -6, 5), 1, 1, 1);
	std::cout << collisionDetector->DetectCollision() << std::endl;
	std::cout << "Test 3" << std::endl;
	collisionDetector->Initialize(glm::vec3(0), glm::vec3(-10, 0, 0), glm::vec3(-10, 0, 0), glm::vec3(-2, -500, 0), glm::vec3(-2, 500, 0), 1, 1, 1);
	std::cout << collisionDetector->DetectCollision() << std::endl;
	std::cout << "Test 4" << std::endl;
	collisionDetector->Initialize(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(-500, -500, 15), glm::vec3(-500, -500, 5), glm::vec3(-100, -10000, 5), 1, 1, 1);
	std::cout << collisionDetector->DetectCollision() << std::endl;*/
}