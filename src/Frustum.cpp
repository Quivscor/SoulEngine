#include "Frustum.h"

Frustum::Frustum()
{

}

Frustum::~Frustum()
{

}

bool Frustum::Intersects(glm::vec3 point, float radius) {
	for (int i = 0; i < 6; i++) {
		if (planes[i].Distance(point) < -radius)
			return false;
	}
	return true;
}

//bool Frustum::Intersects(Collider collider) {
//	int size = collider.polygon.points.size();
//	for (int p = 0; p < 6; p++) {
//		for (int r = 0; r < size; r++)
//		{
//			if (planes[p].Distance(collider.polygon.points[r]) < 0)
//				continue;
//			return(0);
//		}
//	}
//	return(1);
//}
