#include "Plane.h"

void Plane::CreateFromPoints(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2) {
	glm::vec3 v1 = p1 - p0;
	glm::vec3 v2 = p2 - p0;

	abc = glm::normalize(glm::cross(v1, v2));

	d = -glm::dot(p0, abc);
}

float Plane::Distance(glm::vec2 point) {
	return(abc.x * point.x + abc.y * point.y + d);
}

float Plane::Distance(glm::vec3 point) {
	return(abc.x * point.x + abc.y * point.y + abc.z * point.z + d);
}
