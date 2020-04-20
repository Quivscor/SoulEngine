#include "CollisionDetector.h"

void CollisionDetector::Initialize(glm::vec3 position, glm::vec3 velocity, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float ellipsoidMaxX, float ellipsoidMaxY, float ellipsoidMaxZ)
{
	ellipsoidPositionIR = position;
	velocityR = velocity;
	triangleP1R = p1;
	triangleP2R = p2;
	triangleP3R = p3;

	toESpaceMatrix[0][0] = 1 / ellipsoidMaxX;
	toESpaceMatrix[1][1] = 1 / ellipsoidMaxY;
	toESpaceMatrix[2][2] = 1 / ellipsoidMaxZ;
}

void CollisionDetector::SendValuesToESpace()
{
	triangleP1E = toESpaceMatrix * triangleP1R;
	triangleP2E = toESpaceMatrix * triangleP2R;
	triangleP3E = toESpaceMatrix * triangleP3R;

	velocityE = toESpaceMatrix * velocityR;

	ellipsoidPositionIE = toESpaceMatrix * ellipsoidPositionIR;
}

void CollisionDetector::ConstructCollisionValues()
{
	P2P1E = triangleP1E - triangleP2E;
	P2P3E = triangleP3E - triangleP2E;

	planeNormal = glm::cross(P2P3E, P2P1E);

	planeConstantD = (planeNormal.x * triangleP2E.x * -1) + (planeNormal.y * triangleP2E.y * -1) + (planeNormal.z * triangleP2E.z * -1);
	planeConstantP = (float)(planeConstantD / (glm::sqrt((planeNormal.x * planeNormal.x) + (planeNormal.y * planeNormal.y) + (planeNormal.z * planeNormal.z))));
	planeUnitNormal = glm::normalize(planeNormal);
	distance = glm::dot(planeUnitNormal, ellipsoidPositionIE) + planeConstantP;

	if (glm::dot(planeUnitNormal, velocityE) == 0.0f)
	{
		// object is moving parrarel to the plane
		t0 = 0.0f;
		t1 = 1.0f;
	}
	else
	{
		try
		{
			t0 = (1 - distance) / (glm::dot(planeUnitNormal, velocityE));
		}
		catch (const std::invalid_argument& ia)
		{
			t0 = 0;
			t1 = 1;
		}

		try
		{
			t1 = (-1 - distance) / (glm::dot(planeUnitNormal, velocityE));
		}
		catch (const std::invalid_argument& ia)
		{
			t0 = 0;
			t1 = 1;
		}
	}


	if (t1 < t0)
	{
		float tmp = t1;
		t1 = t0;
		t0 = tmp;
	}
}

bool CollisionDetector::DetectCollision()
{
	SendValuesToESpace();
	ConstructCollisionValues();

	if (glm::dot(planeUnitNormal, velocityE) == 0)
	{
		if (glm::abs(distance) > 1)
		{
			return false;
		}
	}
	else if (((t0 < 0) || (t0 > 1)) && ((t1 < 0) || (t1 > 1)))
	{
		return false;
	}

	planeIntersectionPointE = ellipsoidPositionIE - planeUnitNormal;

	glm::vec3 velocityExt0 = glm::vec3(velocityE.x * t0, velocityE.y * t0, velocityE.z * t0);
	planeIntersectionPointE = planeIntersectionPointE + velocityExt0;

	vertexCollision = false;

	if (CheckPositionWithTriangle(planeIntersectionPointE, triangleP1E, triangleP2E, triangleP3E))
	{
		//collision with surface
		//collisition position = planeIntersectionPointE
		//collision distance
		//std::cout << "Collision point: " << std::endl;
		//std::cout << planeIntersectionPointE.x << " " << planeIntersectionPointE.y << " " << planeIntersectionPointE.z << std::endl;
		return true;
	}
	else
	{
		glm::vec3 ellipsoidVertexDistance;
		glm::vec3 vertexEllipsoidDistance;

		float vertexTime1;
		float vertexTime2;
		float smallestSolutionVertex = 10005;

		glm::vec3 collisionPoint;

		float a = (float)((velocityE.x * velocityE.x) + (velocityE.y * velocityE.y) + (velocityE.z * velocityE.z));

		ellipsoidVertexDistance = ellipsoidPositionIE - triangleP1E;

		float b = 2 * glm::dot(velocityE, ellipsoidVertexDistance);

		vertexEllipsoidDistance = triangleP1E - ellipsoidPositionIE;

		float c = ((vertexEllipsoidDistance.x * vertexEllipsoidDistance.x) + (vertexEllipsoidDistance.y * vertexEllipsoidDistance.y) + (vertexEllipsoidDistance.z * vertexEllipsoidDistance.z)) - 1;

		if (((b*b) - (4 * a * c)) >= 0)
		{
			vertexTime1 = ((-b + (float)glm::sqrt((double) ((b*b) - (4 * a * c)))) / (2 * a));
			vertexTime2 = ((-b - (float)glm::sqrt((double) ((b*b) - (4 * a * c)))) / (2 * a));

			if ((vertexTime1 < vertexTime2) && (vertexTime1 <= t1) && (vertexTime1 >= t0)
				&& ((vertexTime1 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime1;
				collisionPoint = triangleP1E;
			}
			else if ((vertexTime2 < vertexTime1) && (vertexTime2 <= t1) && (vertexTime2 >= t0)
				&& ((vertexTime2 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime2;
				collisionPoint = triangleP1E;
			}

		}

		ellipsoidVertexDistance = ellipsoidPositionIE - triangleP2E;

		b = 2 * glm::dot(velocityE, ellipsoidVertexDistance);

		vertexEllipsoidDistance = triangleP2E - ellipsoidPositionIE;

		c = ((vertexEllipsoidDistance.x * vertexEllipsoidDistance.x) + (vertexEllipsoidDistance.y * vertexEllipsoidDistance.y) + (vertexEllipsoidDistance.z * vertexEllipsoidDistance.z)) - 1;

		if ((b*b) - (4 * a * c) >= 0)
		{
			vertexTime1 = ((-b + (float)glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));
			vertexTime2 = ((-b - (float)glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));

			if ((vertexTime1 < vertexTime2) && (vertexTime1 <= t1) && (vertexTime1 >= t0)
				&& ((vertexTime1 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime1;
				collisionPoint = triangleP2E;
			}
			else if ((vertexTime2 < vertexTime1) && (vertexTime2 <= t1) && (vertexTime2 >= t0)
				&& ((vertexTime2 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime2;
				collisionPoint = triangleP2E;
			}

		}

		ellipsoidVertexDistance = ellipsoidPositionIE - triangleP3E;

		b = 2 * glm::dot(velocityE, ellipsoidVertexDistance);

		vertexEllipsoidDistance = triangleP3E - ellipsoidPositionIE;

		c = ((vertexEllipsoidDistance.x * vertexEllipsoidDistance.x) + (vertexEllipsoidDistance.y * vertexEllipsoidDistance.y) + (vertexEllipsoidDistance.z * vertexEllipsoidDistance.z)) - 1;

		if ((b*b) - (4 * a * c) >= 0)
		{
			vertexTime1 = ((-b + (float)glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));
			vertexTime2 = ((-b - (float)glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));

			if ((vertexTime1 < vertexTime2) && (vertexTime1 <= t1) && (vertexTime1 >= t0)
				&& ((vertexTime1 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime1;
				collisionPoint = triangleP3E;
			}
			else if ((vertexTime2 < vertexTime1) && (vertexTime2 <= t1) && (vertexTime2 >= t0)
				&& ((vertexTime2 < smallestSolutionVertex) || (smallestSolutionVertex == 10005)))
			{
				smallestSolutionVertex = vertexTime2;
				collisionPoint = triangleP3E;
			}

		}

		if (smallestSolutionVertex != 10005) 
		{
			//std::cout << "Collision point: " << std::endl;
			//std::cout << collisionPoint.x << " " << collisionPoint.y << " " << collisionPoint.z << std::endl;

			float vertexDistance = smallestSolutionVertex * velocityE.length();

			vertexCollision = true;
		}

		glm::vec3 edge;
		glm::vec3 baseToVertex;
		glm::vec3 edgeIntersectionPoint;

		float intersectionDistance;
		glm::vec3 fromEdgePoint;
		glm::vec3 smallestEdge;

		float edgeTime1;
		float edgeTime2;
		float smallestSolutionEdge = 10005;
		float smallerSolutionEdge;
		float smallerF = -1;
		float smallestF;

		edge = triangleP2E - triangleP1E;
		baseToVertex = triangleP1E - ellipsoidPositionIE;
		a = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * -1 * ((velocityE.x * velocityE.x) + (velocityE.y * velocityE.y) + (velocityE.z * velocityE.z))
			+ (glm::dot(edge, velocityE) * glm::dot(edge, velocityE));
		b = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * 2 * glm::dot(velocityE, baseToVertex)
			- (2 * glm::dot(edge, velocityE) * glm::dot(edge, baseToVertex));
		c = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * (1 - (baseToVertex.x * baseToVertex.x) + (baseToVertex.y * baseToVertex.y) + (baseToVertex.z * baseToVertex.z))
			+ (glm::dot(edge, baseToVertex) * glm::dot(edge, baseToVertex));// / (edge.length() * edge.length()));

		if ((b*b) - (4 * a * c) >= 0)
		{
			edgeTime1 = (float)((-b + glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));
			edgeTime2 = (float)((-b - glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));

			if (edgeTime1 <= edgeTime2)
			{
				smallerSolutionEdge = edgeTime1;
			}
			else
			{
				smallerSolutionEdge = edgeTime2;
			}

			if (smallerSolutionEdge >= 0 && smallerSolutionEdge <= 1)
			{
				smallerF = ((glm::dot(edge, velocityE) * smallerSolutionEdge) - glm::dot(edge, baseToVertex)) / ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z));
			}

			if ((smallerF >= 0 && smallerF <= 1) && (smallerSolutionEdge < smallestSolutionEdge))
			{
				smallestF = smallerF;
				smallestSolutionEdge = smallerSolutionEdge;
				fromEdgePoint = triangleP1E;
				smallestEdge = edge;
			}
		}

		edge = triangleP3E - triangleP2E;
		baseToVertex = triangleP2E - ellipsoidPositionIE;
		a = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * -1 * ((velocityE.x * velocityE.x) + (velocityE.y * velocityE.y) + (velocityE.z * velocityE.z))
			+ (glm::dot(edge, velocityE) * glm::dot(edge, velocityE));
		b = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * 2 * glm::dot(velocityE, baseToVertex)
			- (2 * glm::dot(edge, velocityE) * glm::dot(edge, baseToVertex));
		c = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * (1 - (baseToVertex.x * baseToVertex.x) + (baseToVertex.y * baseToVertex.y) + (baseToVertex.z * baseToVertex.z))
			+ (glm::dot(edge, baseToVertex) * glm::dot(edge, baseToVertex));// / (edge.length() * edge.length()));

		if ((b*b) - (4 * a * c) >= 0)
		{
			edgeTime1 = (float)((-b + glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));
			edgeTime2 = (float)((-b - glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));

			if (edgeTime1 <= edgeTime2)
			{
				smallerSolutionEdge = edgeTime1;
			}
			else
			{
				smallerSolutionEdge = edgeTime2;
			}

			if (smallerSolutionEdge >= 0 && smallerSolutionEdge <= 1)
			{
				smallerF = ((glm::dot(edge, velocityE) * smallerSolutionEdge) - glm::dot(edge, baseToVertex)) / ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z));
			}

			if ((smallerF >= 0 && smallerF <= 1) && (smallerSolutionEdge < smallestSolutionEdge))
			{
				smallestF = smallerF;
				smallestSolutionEdge = smallerSolutionEdge;
				fromEdgePoint = triangleP2E;
				smallestEdge = edge;
			}
		}

		edge = triangleP1E - triangleP3E;
		baseToVertex = triangleP3E - ellipsoidPositionIE;
		a = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * -1 * ((velocityE.x * velocityE.x) + (velocityE.y * velocityE.y) + (velocityE.z * velocityE.z))
			+ (glm::dot(edge, velocityE) * glm::dot(edge, velocityE));
		b = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * 2 * glm::dot(velocityE, baseToVertex)
			- (2 * glm::dot(edge, velocityE) * glm::dot(edge, baseToVertex));
		c = ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z)) * (1 - (baseToVertex.x * baseToVertex.x) + (baseToVertex.y * baseToVertex.y) + (baseToVertex.z * baseToVertex.z))
			+ (glm::dot(edge, baseToVertex) * glm::dot(edge, baseToVertex));// / (edge.length() * edge.length()));

		if ((b*b) - (4 * a * c) >= 0)
		{
			edgeTime1 = (float)((-b + glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));
			edgeTime2 = (float)((-b - glm::sqrt((double)((b*b) - (4 * a * c)))) / (2 * a));

			if (edgeTime1 <= edgeTime2)
			{
				smallerSolutionEdge = edgeTime1;
			}
			else
			{
				smallerSolutionEdge = edgeTime2;
			}

			if (smallerSolutionEdge >= 0 && smallerSolutionEdge <= 1)
			{
				smallerF = ((glm::dot(edge, velocityE) * smallerSolutionEdge) - glm::dot(edge, baseToVertex)) / ((edge.x * edge.x) + (edge.y * edge.y) + (edge.z * edge.z));
			}

			if ((smallerF >= 0 && smallerF <= 1) && (smallerSolutionEdge < smallestSolutionEdge))
			{
				smallestF = smallerF;
				smallestSolutionEdge = smallerSolutionEdge;
				fromEdgePoint = triangleP3E;
				smallestEdge = edge;
			}
		}

		if (smallestSolutionEdge != 10005)
		{


			edgeIntersectionPoint = fromEdgePoint + edge * glm::vec3(smallestF, smallestF, smallestF);
			intersectionDistance = smallestSolutionEdge * velocityE.length();

			//std::cout << "Collision point: " << std::endl;
			//std::cout << edgeIntersectionPoint.x << " " << edgeIntersectionPoint.y << " " << edgeIntersectionPoint.z << std::endl;

			vertexCollision = true;
		}

		//smallerF = ((glm::dot(edge, velocityE) * smallestSolutionEdge) - glm::dot(edge, baseToVertex)) / (edge.length * edge.length);
	} //end of the vertex/edge parian of this method

	if (vertexCollision)
	{
		return true;
	}

	return false;
}

bool CollisionDetector::CheckPositionWithTriangle(glm::vec3 position, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	float angles = 0;

	glm::vec3 v1;
	v1 = position - p1;
	glm::vec3 v2;
	v2 = position - p2;
	glm::vec3 v3;
	v3 = position - p3;

	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);
	v3 = glm::normalize(v3);

	angles += glm::acos(glm::dot(v1, v2));
	angles += glm::acos(glm::dot(v1, v3));
	angles += glm::acos(glm::dot(v3, v2));

	return (glm::abs(angles - 2 * glm::pi<float>()) <= 0.005f);


}