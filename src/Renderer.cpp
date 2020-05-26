#include "Renderer.h"
 #include "Model.h"
#include "TextRendering.h"
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
	glm::mat4 text_matrix_2D = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
	glm::mat4 translate_2d_text = glm::translate(glm::mat4(), glm::vec3(20.0f, 65.0f, .0f));
	glm::mat4 scale_2d_text = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
	TextRendering::Instance()->draw("Poruszanie - WASD", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_2D);
	
		
		std::shared_ptr<Transform> trns = m_Entities[0]->GetComponent<Transform>();
		glm::mat4 scale = glm::scale(trns->GetGlobalMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 set_text_to_origin = glm::translate(glm::mat4(), glm::vec3(-1.8f, -0.4f, 0.0f));
		glm::mat4 text_rotate_y = glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 text_rotate_x = glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 text_translate_to_model_1 = glm::translate(glm::mat4(), glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 50.0f, trns->GetLocalPosition().z));

		glm::mat4 text_matrix_3D_model_1 = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->GetGlobalMatrix() * trns->GetGlobalMatrix()*scale;
		
		TextRendering::Instance()->draw("Smiec", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_3D_model_1);
	
}

void Renderer::LateUpdate() const
{
	glfwSwapBuffers(Window::GetInstance()->GetMWindow());
}

void Renderer::DrawMeshes() const
{
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		// text 3D
		//if (m_Entities[i]->GetComponent<Model>() != nullptr)
		
		
		
		if (m_Entities[i]->GetComponent<Mesh>() != nullptr)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			int anyTexture = 0;
		
			
			Shader* shader = m_Entities[i]->GetComponent<Mesh>()->material->GetShader();
			std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();
		//	TextRendering::Instance()->draw("Agent_1", glm::vec3(0.1f, 1.0f, 0.0f), trns->GetGlobalMatrix());
	
			{
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

					glBindTexture(GL_TEXTURE_2D, mesh->material->GetTextures()[j].id);
					glUniform1i(glGetUniformLocation(shader->ID, ("material." + name + number).c_str()), j);
				}

				shader->use();
				if (m_Entities[i]->GetComponent<Model>() != nullptr)
				{
					m_Entities[i]->GetComponent<Model>()->initShaders(shader);
					m_Entities[i]->GetComponent<Model>()->ChangeBonePositions();
				
					
					//m_Entities[i]->GetComponent<Mesh>()->material->SetShader(defaultShader);
				}
				glUniform3f(glGetUniformLocation(shader->ID, "view_pos"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);
				glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), 32.0f);
				glUniform1f(glGetUniformLocation(shader->ID, "material.transparency"), 1.0f);
				// Point Light 1
				glUniform3f(glGetUniformLocation(shader->ID, "point_light.position"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);

				glUniform3f(glGetUniformLocation(shader->ID, "point_light.ambient"), 0.1f, 0.1f, 0.1f);
				glUniform3f(glGetUniformLocation(shader->ID, "point_light.diffuse"), 1.0f, 1.0f, 1.0f);
				glUniform3f(glGetUniformLocation(shader->ID, "point_light.specular"), 1.0f, 1.0f, 1.0f);

				glUniform1f(glGetUniformLocation(shader->ID, "point_light.constant"), 1.0f);
				glUniform1f(glGetUniformLocation(shader->ID, "point_light.linear"), 0.007);
				glUniform1f(glGetUniformLocation(shader->ID, "point_light.quadratic"), 0.0002);

				glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->GetGlobalMatrix() * trns->GetGlobalMatrix();

				unsigned int model = glGetUniformLocation(shader->ID, "M_matrix");
				glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(trns->GetGlobalMatrix()));

				unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

				glUniform1f(glGetUniformLocation(shader->ID, "waveTime"), (float)TimeCustom::GetTime());

				unsigned int colorLoc = glGetUniformLocation(shader->ID, "color");
				glUniform3fv(colorLoc, 1, glm::value_ptr(mesh->material->GetColor()));
				glUniformMatrix4fv(glGetUniformLocation(shader->ID, "M_matrix"), 1, GL_FALSE, glm::value_ptr(trns->GetGlobalMatrix()));
				glm::mat4 matr_normals_cube = glm::mat4(glm::transpose(glm::inverse(trns->GetGlobalMatrix())));
				glUniformMatrix4fv(glGetUniformLocation(shader->ID, "normals_matrix"), 1, GL_FALSE, glm::value_ptr(matr_normals_cube));
				unsigned int hasTexture = glGetUniformLocation(shader->ID, "hasTexture");
				glUniform1i(hasTexture, anyTexture);
				// draw mesh
				glBindVertexArray(mesh->GetVAO());
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				glActiveTexture(GL_TEXTURE0);
				//glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size());
				for (int i = 0; i < mesh->material->GetTextures().size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}

			}
			
		}

		if (debugMode)
		{
			std::shared_ptr<Collider> collider = m_Entities[i]->GetComponent<Collider>();

			if (collider != nullptr && collider->enabled == true)
			{
				DrawColliders(collider, trns);

			
			}
		
		}
	}
	
}

void Renderer::DrawColliders(std::shared_ptr<Collider> col, std::shared_ptr<Transform> trns) const
{
	Shader* shader = defaultShader;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int pointsCount = (col->polygon.points.size() + 1) * 3;
	
	float* vertices = new float[pointsCount];

	for (int i = 0; i < col->polygon.points.size(); i++)
	{
		vertices[i * 3] = col->polygon.points[i].x;
		vertices[i * 3 + 1] = 0.0f;
		vertices[i * 3 + 2] = col->polygon.points[i].y;

		//std::cout << vertices[i * 3] << " " << vertices[i * 3 + 1] << " " << vertices[i * 3 + 2] << " " << std::endl;
	}

	vertices[pointsCount - 3] = col->polygon.points[0].x;
	vertices[pointsCount - 2] = 0.0f;
	vertices[pointsCount - 1] = col->polygon.points[0].y;

	//std::cout << vertices[pointsCount - 3] << " " << vertices[pointsCount - 2] << " " << vertices[pointsCount - 1] << " " << std::endl;
	//std::cout << "Break" << std::endl;

	/*float vertices[] = {
		 -5.0,  0.0f, -5.0,
		-5.0, 0.0f, 5.0,
		 5.0, 0.0f, 5.0,
		 5.0,  0.0f, -5.0,
		 -5.0,  0.0f, -5.0
	};*/


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointsCount * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->use();

	glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->GetGlobalMatrix();// *trns->matrix;

	unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	unsigned int colorLoc = glGetUniformLocation(shader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0, 0.5f, 0.0f)));

	unsigned int hasTexture = glGetUniformLocation(shader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);

	glDrawArrays(GL_LINE_STRIP, 0, col->polygon.shape.size() + 1);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Renderer::DrawCube(std::shared_ptr<Transform> transform, std::shared_ptr<Material> material)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

	glm::mat4 mvp = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->GetGlobalMatrix() * transform->GetGlobalMatrix();

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