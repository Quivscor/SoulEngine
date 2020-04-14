#include "Renderer.h"

Renderer::Renderer(Shader* defaultShader) : defaultShader(defaultShader)
{

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
	for (int i = 0; i < meshes.size(); i++)
	{

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < meshes[i].textures.size(); i++)
		{
			defaultShader->setInt("material.diffuse", i);
			glActiveTexture(GL_TEXTURE0 + i); //glActiveTexture(diffuse_textureN), where N = GL_TEXTURE0 + i

			std::string number;
			std::string name = meshes[i].textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			defaultShader->setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, meshes[i].textures[i].id);
		}

		defaultShader->use();

		glm::mat4 localTransform = glm::mat4(1);
		localTransform = glm::scale(localTransform, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 mvp = camProjection->GetProjection() * camView->matrix * localTransform;

		unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		// draw mesh
		glBindVertexArray(meshes[i].GetVAO());
		glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size());
	}
}

void Renderer::DrawCube(Transform* transform, Material* material)
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

	glm::mat4 mvp = camProjection->GetProjection() * camView->matrix * transform->matrix;

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	unsigned int colorLoc = glGetUniformLocation(defaultShader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(material->GetColor()));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Renderer::DebugSetProjectionView(Transform* view, Camera* projection)
{
	camProjection = projection;
	camView = view;
}