#pragma once
#include "Core.h"
#include "Renderer.h"

struct ColliderPolygon {
	std::vector<glm::vec2> points;
	std::vector<glm::vec2> shape;
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

private:
	ColliderPolygon polygon;
};