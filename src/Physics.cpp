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
		if (!m_Entities[i]->isActive)
			continue;

		transform = m_Entities[i]->GetComponent<Transform>();
		collider = m_Entities[i]->GetComponent<Collider>();

		if (transform->IsDirty())
		{
			glm::mat4 matrix = glm::mat4(1.0f);
			matrix = glm::translate(matrix, transform->GetLocalPosition());
			matrix = glm::rotate(matrix, (transform->GetLocalRotation().x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
			matrix = glm::rotate(matrix, (transform->GetLocalRotation().y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
			matrix = glm::rotate(matrix, (transform->GetLocalRotation().z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));
			matrix = glm::scale(matrix, transform->GetLocalScale());
			transform->SetLocalMatrix(matrix);

			//collider shape depends on entity 

			//update collider position
			if (collider != nullptr)
			{
				glm::vec4 point;

				glm::mat4 colliderMatrix = glm::mat4(1.0f);
				colliderMatrix = glm::translate(colliderMatrix, transform->GetGlobalPosition());
				colliderMatrix = glm::rotate(colliderMatrix, (transform->GetGlobalRotation().y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
				colliderMatrix = glm::scale(colliderMatrix, transform->GetGlobalScale());

				for (int i = 0; i < collider->polygon.shape.size(); i++)
				{
					point = colliderMatrix *  glm::vec4(collider->polygon.shape[i].x, 0.0f, collider->polygon.shape[i].y, 1.0f);
					collider->polygon.points[i] = glm::vec2(point.x, point.z);
				}
			}
			transform->SetDirty(false);
		}

		if (collider != nullptr)
		{
			colliders.push_back(collider);
			collidersTransforms.push_back(transform);
		}
	}

	bool shownDebugInfo = false;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		transform = m_Entities[i]->GetComponent<Transform>();

		collider = m_Entities[i]->GetComponent<Collider>();

		if (collider != nullptr)
		{
			MoveCollider(collider, transform);
		}

		if (transform->GetMoveVector() == glm::vec3(0) && transform->GetRotateVector() == glm::vec3(0))
		{
			transform->ShowDebugInfo();
			shownDebugInfo = true;
			continue;
		}

		glm::mat4 matrix;

		glm::mat4 translateToPivot = glm::mat4(1);
		glm::mat4 translateFromPivot = glm::mat4(1);

		glm::mat4 rotateMatrix = glm::mat4(1);
		rotateMatrix = glm::rotate(rotateMatrix, (transform->GetRotateVector().x * 3.14f / 180), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateMatrix = glm::rotate(rotateMatrix, (transform->GetRotateVector().y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateMatrix = glm::rotate(rotateMatrix, (transform->GetRotateVector().z * 3.14f / 180), glm::vec3(0.0f, 0.0f, 1.0f));

		translateToPivot = glm::translate(glm::mat4(1.0f), (transform->GetMoveVector()) * (-1.0f));
		translateFromPivot = glm::translate(glm::mat4(1.0f), (transform->GetMoveVector()));

		matrix = glm::translate(transform->GetLocalMatrix(), (transform->GetMoveVector()));
		matrix = matrix * (translateToPivot * rotateMatrix * translateFromPivot);
		transform->SetLocalMatrix(matrix);

		transform->SetLocalPosition(glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]));
		transform->SetLocalRotation(transform->GetLocalRotation() + transform->GetRotateVector());

		//transform->matrix = glm::translate(transform->matrix, transform->moveVector);
		transform->SetMoveVector(glm::vec3(0));
		transform->SetRotateVector(glm::vec3(0));

		if (shownDebugInfo == false)
		{
			transform->ShowDebugInfo();
		}
	}

	//prepare for triggers function
	for (int i = 0; i < colliders.size(); i++)
		colliders[i]->IncreaseControlFlag();

	for (int i = 0; i < colliders.size(); i++)
	{
		for (int j = i + 1; j < colliders.size(); j++)
		{
			CheckCollisions(colliders[i], colliders[j], collidersTransforms[i], collidersTransforms[j]);			
		}
	}

	//check triggers function
	for (int i = 0; i < colliders.size(); i++)
		colliders[i]->CheckControlFlags();
}

bool Physics::CheckCollisions(std::shared_ptr<Collider> col1, std::shared_ptr<Collider> col2, std::shared_ptr<Transform> trns1, std::shared_ptr<Transform> trns2) const
{
	if (col1->isStatic && col2->isStatic)
		return true;

	if (!col1->enabled || !col2->enabled)
		return true;

	if (EntityManager::GetInstance()->GetEntity(col1->GetOwnerID())->layer == EntityManager::GetInstance()->GetEntity(col2->GetOwnerID())->layer == EnemyLayer)
		return true;

	Collider* col1ref = col1.get();
	Collider* col2ref = col2.get();

	bool isTrigger = col1ref->isTrigger || col2ref->isTrigger;

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

			if (!isTrigger)
				overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			{
				return false;
			}
		}
	}

	//all code below execute when two objects collide with each other
	//if anyone is trigger then calculate response
	if (!isTrigger)
	{
		glm::vec2 d = { trns2->GetGlobalPositionFromMatrix().x - trns1->GetGlobalPositionFromMatrix().x, trns2->GetGlobalPositionFromMatrix().z - trns1->GetGlobalPositionFromMatrix().z };
		float s = sqrtf(d.x * d.x + d.y * d.y);

		glm::mat4 tmpMatrix = trns1->GetLocalMatrix();
		tmpMatrix[3][0] -= (overlap * d.x / s);
		tmpMatrix[3][2] -= (overlap * d.y / s);

		trns1->SetLocalMatrix(tmpMatrix);
		trns1->SetLocalPosition(glm::vec3(tmpMatrix[3][0], tmpMatrix[3][1], tmpMatrix[3][2]));
	}
	//for triggers
	else
	{
		if (col1->isTrigger || col2->isTrigger) 
		{
			col1->AddTriggerCollision(col2);
			col2->AddTriggerCollision(col1);
		}

	}

	return false;
}

void Physics::MoveCollider(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const
{
	glm::mat4 colliderMatrix = trns->GetGlobalMatrix();
	colliderMatrix = glm::translate(colliderMatrix, trns->GetMoveVector() * (-1.0f));
	colliderMatrix = glm::rotate(colliderMatrix, (trns->GetRotateVector().y * 3.14f / 180), glm::vec3(0.0f, 1.0f, 0.0f));
	colliderMatrix = glm::translate(colliderMatrix, trns->GetMoveVector() * 2.0f);

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

}