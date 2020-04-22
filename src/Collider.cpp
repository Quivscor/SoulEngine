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