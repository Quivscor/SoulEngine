#pragma once
#include "Core.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "ScriptableObject.h"

struct ColliderPolygon {
	std::vector<glm::vec2> points;
	std::vector<glm::vec2> shape;
};

class Collider;

struct Collision {
	std::shared_ptr<Collider> collider;
	int controlFlag;
};

class Collider : public Component
{
	friend class Renderer;
	friend class Physics;

public:
	Collider();
	~Collider();

	void SetShape(std::vector<glm::vec2> shape);

	virtual ComponentType GetComponentType() const override { return ComponentType::ColliderComponent; }

	bool isTrigger = false;
	bool isStatic = false;

private:
	ColliderPolygon polygon;

	std::vector<Collision> collisions;

	int controlFlag = 0;

	void AddTriggerCollision(std::shared_ptr<Collider> otherObjCollider);
	void CheckControlFlags();
	void IncreaseControlFlag();
};