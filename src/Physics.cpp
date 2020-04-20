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
			std::shared_ptr<ColliderMesh> colliderMesh = m_Entities[i]->GetComponent<ColliderMesh>();

			//update collider position
			if (colliderMesh != nullptr)
			{
				glm::vec4 point;

				for (int i = 0; i < colliderMesh->originVertices.size(); i++)
				{
					point = transform->matrix *  glm::vec4(colliderMesh->originVertices[i], 1.0f);

					colliderMesh->vertices[i] = glm::vec3(point.x, point.y, point.z);
				}
			}
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
		transform->matrix = glm::translate(transform->matrix, transform->moveVector);

		//transform->matrix = glm::translate(transform->matrix, transform->moveVector);

		//transform->moveVector = glm::vec3(0);
		transform->rotateVector = glm::vec3(0);
	}

	std::vector<std::shared_ptr<ColliderMesh>> colliderMeshes;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		std::shared_ptr<ColliderMesh> cm = m_Entities[i]->GetComponent<ColliderMesh>();

		if (cm != nullptr)
			colliderMeshes.push_back(cm);
	}

	std::shared_ptr<ColliderElipse> ce;
	std::shared_ptr<Transform> trns;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		ce = m_Entities[i]->GetComponent<ColliderElipse>();
		trns = m_Entities[i]->GetComponent<Transform>();

		bool collision = false;

		if (ce != nullptr)
		{
			for (int j = 0; j < colliderMeshes.size(); j++)
			{
				for (int k = 0; k < colliderMeshes[j]->indices.size(); k += 3)
				{
					collisionDetector->Initialize(trns->GetPositionFromMatrix(),
						trns->moveVector,
						colliderMeshes[j]->vertices[colliderMeshes[j]->indices[k]],
						colliderMeshes[j]->vertices[colliderMeshes[j]->indices[k + 1]],
						colliderMeshes[j]->vertices[colliderMeshes[j]->indices[k + 2]],
						ce->radius.x,
						ce->radius.y,
						ce->radius.z);

					collision = collisionDetector->DetectCollision();
				}
			}
		}

		if (!collision)
			trns->matrix = glm::translate(trns->matrix, trns->moveVector);

		trns->moveVector = glm::vec3(0);
	}
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