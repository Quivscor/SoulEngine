#include "Physics.h"

Physics::Physics()
{
	
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
	std::shared_ptr<Collider> collider;

	std::vector < std::shared_ptr<Transform>> collidersTransforms;
	std::vector<std::shared_ptr<Collider>> colliders;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		transform = m_Entities[i]->GetComponent<Transform>();
		collider = m_Entities[i]->GetComponent<Collider>();

		if (collider != nullptr)
		{
			colliders.push_back(collider);
			collidersTransforms.push_back(transform);
		}

		if (transform->dirtyFlag == true)
		{
			transform->matrix = glm::mat4(1.0f);
			transform->matrix = glm::translate(transform->matrix, transform->position);
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
			transform->matrix = glm::rotate(transform->matrix, (transform->rotation.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
			transform->matrix = glm::scale(transform->matrix, transform->scale);

			//collider shape depends on entity 

			//update collider position
			if (collider != nullptr)
			{
				glm::vec4 point;

				glm::mat4 colliderMatrix = glm::mat4(1.0f);
				colliderMatrix = glm::translate(colliderMatrix, transform->position);
				colliderMatrix = glm::rotate(colliderMatrix, (transform->rotation.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
				colliderMatrix = glm::scale(colliderMatrix, transform->scale);

				for (int i = 0; i < collider->polygon.shape.size(); i++)
				{
					point = colliderMatrix *  glm::vec4(collider->polygon.shape[i].x, 0.0f, collider->polygon.shape[i].y, 1.0f);
					collider->polygon.points[i] = glm::vec2(point.x, point.z);
				}
			}
			transform->dirtyFlag = false;
		}
	}

	for (int i = 0; i < m_Entities.size(); i++)
	{
		transform = m_Entities[i]->GetComponent<Transform>();

		if (transform->moveVector == glm::vec3(0) && transform->rotateVector == glm::vec3(0))
			continue;

		collider = m_Entities[i]->GetComponent<Collider>();

		if (collider != nullptr)
		{
			MoveCollider(collider, transform);
		}

		transform->matrix = glm::translate(transform->matrix, transform->moveVector * (-1.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->matrix = glm::rotate(transform->matrix, (transform->rotateVector.z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
		transform->matrix = glm::translate(transform->matrix, transform->moveVector * 2.0f);

		//transform->matrix = glm::translate(transform->matrix, transform->moveVector);
		transform->moveVector = glm::vec3(0);
		transform->rotateVector = glm::vec3(0);
	}

	for (int i = 0; i < colliders.size(); i++)
	{
		for (int j = i + 1; j < colliders.size(); j++)
		{
			if (CheckCollisions(colliders[i], colliders[j], collidersTransforms[i], collidersTransforms[j]))
				std::cout << "Kolizja" << std::endl;
		}
	}
}

bool Physics::CheckCollisions(std::shared_ptr<Collider> col1, std::shared_ptr<Collider> col2, std::shared_ptr<Transform> trns1, std::shared_ptr<Transform> trns2) const
{
	Collider* col1ref = col1.get();
	Collider* col2ref = col2.get();

	float overlap = INFINITY;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			col1ref = col2.get();
			col2ref = col1.get();
		}

		for (int a = 0; a < col1ref->polygon.points.size(); a++)
		{
			int b = (a + 1) % col1ref->polygon.points.size();

			glm::vec2 axisProj = { -(col1ref->polygon.points[b].y - col1ref->polygon.points[a].y), (col1ref->polygon.points[b].x - col1ref->polygon.points[a].x) };

			float min_r1 = INFINITY;
			float max_r1 = -INFINITY;

			for (int p = 0; p < col1ref->polygon.points.size(); p++)
			{
				float q = (col1ref->polygon.points[p].x * axisProj.x + col1ref->polygon.points[p].y * axisProj.y);
				min_r1 = std::min(min_r1, q);
				max_r1 = std::max(max_r1, q);
			}

			float min_r2 = INFINITY;
			float max_r2 = -INFINITY;

			for (int p = 0; p < col2ref->polygon.points.size(); p++)
			{
				float q = (col2ref->polygon.points[p].x * axisProj.x + col2ref->polygon.points[p].y * axisProj.y);
				min_r2 = std::min(min_r2, q);
				max_r2 = std::max(max_r2, q);
			}

			overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			{
				return false;
			}
		}
	}

	glm::vec2 d = { trns2->GetPositionFromMatrix().x - trns1->GetPositionFromMatrix().x, trns2->GetPositionFromMatrix().y - trns1->GetPositionFromMatrix().y };
	float s = sqrtf(d.x * d.x + d.y * d.y);

	trns1->matrix = glm::translate(trns1->matrix, glm::vec3((-1) * (overlap * d.x / s), 0.0f, (-1) * (overlap * d.y / s)));

	glm::vec4 point;

	for (int i = 0; i < col1->polygon.shape.size(); i++)
	{
		point = trns1->matrix * glm::vec4(col1->polygon.shape[i].x, 0.0f, col1->polygon.shape[i].y, 1.0f);
		col1->polygon.points[i] = glm::vec2(point.x, point.z);
	}

	return false;
}

void Physics::MoveCollider(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const
{
	glm::mat4 colliderMatrix = trns->matrix;
	colliderMatrix = glm::translate(colliderMatrix, trns->moveVector * (-1.0f));
	colliderMatrix = glm::rotate(colliderMatrix, (trns->rotateVector.y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
	colliderMatrix = glm::translate(colliderMatrix, trns->moveVector * 2.0f);

	glm::vec4 point;

	for (int i = 0; i < col->polygon.shape.size(); i++)
	{
		point = colliderMatrix * glm::vec4(col->polygon.shape[i].x, 0.0f, col->polygon.shape[i].y, 1.0f);
		col->polygon.points[i] = glm::vec2(point.x, point.z);
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

		transform->matrix = glm::translate(transform->matrix, transform->moveVector);
		transform->moveVector = glm::vec3(0);
		transform->rotateVector = glm::vec3(0);
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