#include "Water.h"

Water::Water() {}

Water::~Water() {}

void Water::CreateShape()
{
	thisEntity->AddComponent<Mesh>();

	for (int x = 0; x < xRectangles; x++)
	{
		for (int z = 0; z < zRectangles; z++)
		{
			Vertex vert;

			vert.Position = glm::vec3(width / xRectangles * x, 0.0f, height / zRectangles * z);
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);

			vert.Position = glm::vec3(width / xRectangles * (x + 1), 0.0f, height / zRectangles * z);
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);

			vert.Position = glm::vec3(width / xRectangles * (x + 1), 0.0f, height / zRectangles * (z + 1));
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);

			vert.Position = glm::vec3(width / xRectangles * x, 0.0f, height / zRectangles * z);
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);

			vert.Position = glm::vec3(width / xRectangles * (x + 1), 0.0f, height / zRectangles * (z + 1));
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);

			vert.Position = glm::vec3(width / xRectangles * x, 0.0f, height / zRectangles * (z + 1));
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);
		}
	}

	glm::vec4 indicator;

	for (int i = 0; i < vertices.size(); i += 6)
	{
		indicator.x = vertices[i + 1].Position.x;
		indicator.y = vertices[i + 1].Position.z;

		indicator.z = vertices[i + 2].Position.x;
		indicator.w = vertices[i + 2].Position.z;

		indicators.push_back(indicator);

		indicator.x = vertices[i + 2].Position.x;
		indicator.y = vertices[i + 2].Position.z;

		indicator.z = vertices[i].Position.x;
		indicator.w = vertices[i].Position.z;

		indicators.push_back(indicator);

		indicator.x = vertices[i].Position.x;
		indicator.y = vertices[i].Position.z;

		indicator.z = vertices[i + 1].Position.x;
		indicator.w = vertices[i + 1].Position.z;

		indicators.push_back(indicator);

		indicator.x = vertices[i + 4].Position.x;
		indicator.y = vertices[i + 4].Position.z;

		indicator.z = vertices[i + 5].Position.x;
		indicator.w = vertices[i + 5].Position.z;

		indicators.push_back(indicator);

		indicator.x = vertices[i + 5].Position.x;
		indicator.y = vertices[i + 5].Position.z;

		indicator.z = vertices[i + 3].Position.x;
		indicator.w = vertices[i + 3].Position.z;

		indicators.push_back(indicator);

		indicator.x = vertices[i + 3].Position.x;
		indicator.y = vertices[i + 3].Position.z;

		indicator.z = vertices[i + 4].Position.x;
		indicator.w = vertices[i + 4].Position.z;

		indicators.push_back(indicator);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec2 currentVertexPos = glm::vec2(vertices[i].Position.x, vertices[i].Position.z);
		glm::vec2 vertex1Pos = glm::vec2(indicators[i].x, indicators[i].y);
		glm::vec2 vertex2Pos = glm::vec2(indicators[i].z, indicators[i].w);
		glm::vec2 offset1 = vertex1Pos - currentVertexPos;
		glm::vec2 offset2 = vertex2Pos - currentVertexPos;

		indicators[i] = glm::vec4(offset1.x, offset1.y, offset2.x, offset2.y);

		//std::cout << "Indicators: " << offset1.x << " " << offset1.y << " " << offset2.x << " " << offset2.y << " " << std::endl;
	}

	/*for (int x = 0; x < xRectangles; x++)
	{
		for (int z = 0; z < zRectangles; z++)
		{
			//first triangle
			indices.push_back(z + x * (zRectangles + 1));
			indices.push_back(z + (x + 1) *  (zRectangles + 1));
			indices.push_back(z + (x + 1) *  (zRectangles + 1) + 1);

			//second triangle
			indices.push_back(z + x * (zRectangles + 1));
			indices.push_back(z + (x + 1) *  (zRectangles + 1) + 1);
			indices.push_back(z + x * (zRectangles + 1) + 1);
		}
	}*/

	thisEntity->GetComponent<Mesh>()->vertices = this->vertices;
	thisEntity->GetComponent<Mesh>()->indices = this->indices;

	Shader* shader = new Shader("./res/shaders/water.vert", "./res/shaders/water.frag");

	//thisEntity->AddComponent<Material>();
	//thisEntity->GetComponent<Material>()->SetShader(shader);

	thisEntity->GetComponent<Mesh>()->material = new Material();
	thisEntity->GetComponent<Mesh>()->material->SetColor(glm::vec3(0, 0, 1));
	thisEntity->GetComponent<Mesh>()->material->SetShader(shader);

	thisEntity->GetComponent<Mesh>()->setupMesh(indicators);
}