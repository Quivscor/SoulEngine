#include "Water.h"

Water::Water() {}

Water::~Water() {}

void Water::CreateShape()
{
	thisEntity->AddComponent<Mesh>();

	for (int x = 0; x < xRectangles + 1; x++)
	{
		for (int z = 0; z < zRectangles + 1; z++)
		{
			Vertex vert;
			vert.Position = glm::vec3(width / xRectangles * x, 0.0f, height / zRectangles * z);
			vert.Normal = glm::vec3(0, 0, 0);
			vert.TexCoords = glm::vec2(0, 0);

			vertices.push_back(vert);
		}
	}

	for (int x = 0; x < xRectangles; x++)
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
	}

	thisEntity->GetComponent<Mesh>()->vertices = this->vertices;
	thisEntity->GetComponent<Mesh>()->indices = this->indices;

	Shader* shader = new Shader("./res/shaders/water.vert", "./res/shaders/water.frag");

	//thisEntity->AddComponent<Material>();
	//thisEntity->GetComponent<Material>()->SetShader(shader);

	thisEntity->GetComponent<Mesh>()->material = new Material();
	thisEntity->GetComponent<Mesh>()->material->SetColor(glm::vec3(0, 0, 1));
	thisEntity->GetComponent<Mesh>()->material->SetShader(shader);

	thisEntity->GetComponent<Mesh>()->setupMesh();
}