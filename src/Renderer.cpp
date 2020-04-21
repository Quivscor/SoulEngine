#include "Renderer.h"

Shader* Renderer::defaultShader = nullptr;

Renderer::Renderer(Shader* shader)
{
	defaultShader = shader;
}

Renderer::~Renderer()
{

}

void Renderer::Init() const
{

}

void Renderer::Update() const
{
	glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	DrawMeshes();
}

void Renderer::LateUpdate() const
{
	glfwSwapBuffers(Window::GetInstance()->GetMWindow());
}

void Renderer::DrawMeshes() const
{
	std::shared_ptr<ColliderMesh> cm;
	std::shared_ptr<ColliderElipse> ce;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		int anyTexture = 0;

		std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();
		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();

		for (unsigned int j = 0; j < mesh->material->GetTextures().size(); j++)
		{
			anyTexture = 1;
			//defaultShader->setInt("material.diffuse", i);
			glActiveTexture(GL_TEXTURE0 + j); //glActiveTexture(diffuse_textureN), where N = GL_TEXTURE0 + i

			std::string number;
			std::string name = mesh->material->GetTextures()[j].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			//defaultShader->setFloat(("material." + name + number).c_str(), i);
			glUniform1i(glGetUniformLocation(defaultShader->ID, (name + number).c_str()), j);
			glBindTexture(GL_TEXTURE_2D, mesh->material->GetTextures()[j].id);
		}

		defaultShader->use();

		glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->matrix * trns->matrix;

		unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
		glUniform3fv(colorLoc, 1, glm::value_ptr(mesh->material->GetColor()));

		unsigned int hasTexture = glGetUniformLocation(defaultShader->ID, "hasTexture");
		glUniform1i(hasTexture, anyTexture);
		// draw mesh
		glBindVertexArray(mesh->GetVAO());
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		//glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size());


		if (debugMode)
		{
			cm = m_Entities[i]->GetComponent<ColliderMesh>();

			if (cm != nullptr)
				DrawDebugModeColliderMesh(cm, trns, mvp);

			ce = m_Entities[i]->GetComponent<ColliderElipse>();

			if (ce != nullptr)
				DrawDebugModeColliderElipse(ce, trns, mvp);
		}

	}
}

void Renderer::DrawDebugModeColliderMesh(std::shared_ptr<ColliderMesh> cm, std::shared_ptr<Transform> trns, glm::mat4 mvp) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/*unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, cm->originVertices.size() * sizeof(float), &cm->originVertices, GL_STATIC_DRAW);

	for (int i = 0; i < cm->vertices.size(); i++)
	{
		std::cout << cm->originVertices[i].x << " " << cm->originVertices[i].y << " " << cm->originVertices[i].z << " " << std::endl;
	}
	std::cout << "BREAK" << std::endl;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cm->indices.size() * sizeof(unsigned int),
		&cm->indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);

	defaultShader->use();

	//glm::mat4 mvp2 = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->matrix;// *trns->matrix;

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	unsigned int hasTexture = glGetUniformLocation(defaultShader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, cm->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);*/

	defaultShader->use();

	//glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->matrix * trns->matrix;

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	unsigned int hasTexture = glGetUniformLocation(defaultShader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);
	// draw mesh
	glBindVertexArray(cm->GetVAO());
	glDrawElements(GL_TRIANGLES, cm->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	//glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size());
}

void Renderer::DrawDebugModeColliderElipse(std::shared_ptr<ColliderElipse> ce, std::shared_ptr<Transform> trns, glm::mat4 mvp) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float x = trns->GetPositionFromMatrix().x;
	float y = trns->GetPositionFromMatrix().y;
	float z = trns->GetPositionFromMatrix().z;
	float rX = ce->radius.x;
	float rY = ce->radius.y;
	float rZ = ce->radius.z;

	float vertices[] = {
	x + rX * (-1), y + rY * (-1), z,
	x + rX, y + rY * (-1), z,
	x + rX * (-1), y + rY, z,
	x + rX, y + rY * (-1), z,
	x + rX, y + rY, z,
	x + rX * (-1), y + rY, z,

	x, y + rY * (-1), z + rZ * (-1),
	x, y + rY * (-1), z + rZ,
	x, y + rY, z,
	x, y + rY * (-1), z,
	x, y + rY, z + rZ * (-1),
	x, y + rY, z + rZ,
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	defaultShader->use();

	glm::mat4 mvp2 = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->matrix;// *transform->matrix;

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp2));

	unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));

	unsigned int hasTexture = glGetUniformLocation(defaultShader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Renderer::DrawCube(std::shared_ptr<Transform> transform, std::shared_ptr<Material> material)
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	defaultShader->use();

	glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->matrix * transform->matrix;

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(material->GetColor()));

	unsigned int hasTexture = glGetUniformLocation(defaultShader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

Shader* Renderer::GetDefualtShader()
{
	return Renderer::defaultShader;
}

void Renderer::SetCamera(std::shared_ptr<Entity> camera)
{
	mainCamera = camera;
	//camProjection = camera->GetComponent<Camera>()->GetProjection();
	//camView = camera->GetComponent<Transform>()->matrix;
}

/*void Renderer::DebugSetProjectionView(Transform* view, Camera* projection)
{
	camProjection = projection;
	camView = view;
}*/