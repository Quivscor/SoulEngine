#pragma once
#include "Core.h"

class Plane
{
public:
	glm::vec3 abc;
	float d;

	void CreateFromPoints(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2);
	float Distance(glm::vec2 point);
	float Distance(glm::vec3 point);
};

