#pragma once

#include "Core.h"
#include "Physics.h"

class CollisionDetector 
{
	friend class Physics;

public:
	void Initialize(glm::vec3 position, glm::vec3 velocity, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float ellipsoidMaxX, float ellipsoidMaxY, float ellipsoidMaxZ);
	void SendValuesToESpace();
	void ConstructCollisionValues();
	bool DetectCollision();
	bool CheckPositionWithTriangle(glm::vec3 position, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

private:
	glm::vec3 ellipsoidPositionIR;
	glm::vec3 ellipsoidPositionIE;
	
	glm::vec3 triangleP1R;
	glm::vec3 triangleP2R;
	glm::vec3 triangleP3R;

	glm::vec3 triangleP1E;
	glm::vec3 triangleP2E;
	glm::vec3 triangleP3E;

	glm::vec3 P2P1E;
	glm::vec3 P2P3E;

	glm::vec3 planeIntersectionPointR;
	glm::vec3 planeIntersectionPointE;

	glm::vec3 planeNormal;
	glm::vec3 planeUnitNormal;

	float planeConstantD;
	float planeConstantP;

	float distance;

	float t0;
	float t1;

	glm::vec3 velocityR;
	glm::vec3 velocityE;

	glm::mat3 toESpaceMatrix;
	glm::mat3 fromSpaceMatrix;

	//collision types?
	//...
	bool vertexCollision = false;
};
