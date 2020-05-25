#pragma once
#include "Plane.h"

class Frustum
{
public:
	Frustum();
	~Frustum();

	bool Intersects(glm::vec3 point, float radius = 10);
	//bool Intersects(Collider collider);

	enum {
		COORD_BOTTOMLEFT = 0,
		COORD_BOTTOMRIGHT,
		COORD_TOPLEFT,
		COORD_TOPRIGHT
	};

	enum {
		PLANE_LEFT = 0,
		PLANE_RIGHT,
		PLANE_BOTTOM,
		PLANE_TOP,
		PLANE_NEAR,
		PLANE_FAR
	};

	//Near and far vertices
	glm::vec3 fnear[4];
	glm::vec3 ffar[4];

	//Center
	glm::vec3 center;
	Plane planes[6];
};
