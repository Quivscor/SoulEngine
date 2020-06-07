#include "Renderer.h"
 #include "Model.h"
#include "TextRendering.h"
#include<Billboard.h>
Shader* Renderer::defaultShader = nullptr;

Renderer::Renderer(Shader* shader)
{
	defaultShader = shader;
	box = new Billboard("./res/textures/ExampleBillboard.DDS", true);
	box2 = new Billboard("./res/textures/stone.jpg", false);
	simpleDepthShader = new Shader("./res/shaders/3.1.3.shadow_mapping_depth.vs", "./res/shaders/3.1.3.shadow_mapping_depth.fs");
	debugDepthQuad = new Shader("./res/shaders/3.1.3.shadow_mapping_depth.vs", "./res/shaders/3.1.3.shadow_mapping_depth.fs");
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	lightPos = glm::vec3(25.0f, 2.0f, 25.f);

}

Renderer::~Renderer()
{

}

void Renderer::Init() const
{
	
}

void Renderer::Update() const
{
	mainCamera->GetComponent<Camera>()->CalculateFrustum();

	glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	DrawMeshes();
	//DrawGrass();

	//DrawFrustum(mainCamera->GetComponent<Camera>()->m_Frustum);
	glm::mat4 text_matrix_2D = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
	glm::mat4 translate_2d_text = glm::translate(glm::mat4(), glm::vec3(20.0f, 65.0f, .0f));
	glm::mat4 scale_2d_text = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
	TextRendering::Instance()->draw("Poruszanie - WASD", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_2D);
	
	
	std::shared_ptr<Transform> trns = m_Entities[0]->GetComponent<Transform>();
	//std::cout << trns->GetLocalPosition().x << std::endl;
	box->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 1.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
	box2->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 2.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
	/*Billboard::Instance("./res/textures/ExampleBillboard.DDS", true,mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 1.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
	Billboard::Instance("./res/textures/stone.jpg", false, mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 2.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));*/
	/*Billboard::Instance("./res/textures/ExampleBillboard.DDS", true)->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 1.5f, trns->GetLocalPosition().z - 0.f),glm::vec2(1.0f,0.125f));

	Billboard::Instance("./res/textures/stone.jpg", false)->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 2.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));*/
	glm::mat4 scale = glm::scale(trns->GetLocalMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
	//glm::mat4 set_text_to_origin = glm::translate(trns->GetLocalMatrix(), glm::vec3(-2.f, 20.0f, 0.0f));

	glm::mat4 text_rotate_y = glm::rotate(glm::mat4(), glm::radians(-trns->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 text_translate_to_model_1 = glm::translate(trns->GetLocalMatrix(), glm::vec3(trns->GetLocalPosition().x - 2.f, trns->GetLocalPosition().y + 20.0f, trns->GetLocalPosition().z));

	glm::mat4 text_matrix_3D_model_1 = mainCamera->GetComponent<Camera>()->GetProjection() * mainCamera->GetComponent<Transform>()->GetGlobalMatrix() * text_translate_to_model_1 * scale;

	TextRendering::Instance()->draw("Gracz", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_3D_model_1);
}

void Renderer::LateUpdate() const
{
	glfwSwapBuffers(Window::GetInstance()->GetMWindow());
}

void Renderer::DrawMeshes() const
{
	
	int modelsDrawnCount = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 50.0f, -10.0f, 50.0f, near_plane, far_plane);
	//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), mainCamera->GetComponent<Camera>()->upVector);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

	lightSpaceMatrix = lightProjection * lightView;


	simpleDepthShader->use();
	simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();

		if (mainCamera->GetComponent<Camera>()->DistanceFromCameraTarget(trns) > 14.0f)
		{
			continue;
		}

		
		simpleDepthShader->setMat4("model", trns->GetGlobalMatrix());
		if (mesh != nullptr)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			int anyTexture = 0;


			// render scene from light's point of view

		
			//  glActiveTexture(GL_TEXTURE0);
			 // glBindTexture(GL_TEXTURE_2D, mesh->material->GetTextures()[0].id);
			
			modelsDrawnCount++;

			glBindVertexArray(mesh->GetVAO());

			if (mesh->hasEBO)
			{
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
			}

			glBindVertexArray(0);





		}
		
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();

		if (mainCamera->GetComponent<Camera>()->DistanceFromCameraTarget(trns) > 14.0f)
		{
			continue;
		}
			
		modelsDrawnCount++;

		if (mesh != nullptr)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			int anyTexture = 0;
		
			
			Shader* shader = m_Entities[i]->GetComponent<Mesh>()->material->GetShader();
			
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

		
			glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, depthMap);
			/*		glUniform1i(glGetUniformLocation(shader->ID, "material.shadowMap"), mesh->material->GetTextures().size());

					unsigned int xd = glGetUniformLocation(shader->ID, "lightSpaceMatrix");
					glUniformMatrix4fv(xd, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));*/
			shader->use();
			shader->setVec3("dir_light.direction", lightPos);
			shader->setInt("material.shadowMap", 2);
					shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
			if (m_Entities[i]->GetComponent<Model>() != nullptr)
			{
				m_Entities[i]->GetComponent<Model>()->initShaders(shader);
				m_Entities[i]->GetComponent<Model>()->ChangeBonePositions();
				
					
				//m_Entities[i]->GetComponent<Mesh>()->material->SetShader(defaultShader);
			}
			glUniform3f(glGetUniformLocation(shader->ID, "view_pos"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);
			glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), 0.5f);
			glUniform1f(glGetUniformLocation(shader->ID, "material.transparency"), 1.0f);

			// Point Light 1
			glUniform3f(glGetUniformLocation(shader->ID, "point_light.position"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);

			glUniform3f(glGetUniformLocation(shader->ID, "point_light.ambient"),  -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(shader->ID, "point_light.diffuse"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(shader->ID, "point_light.specular"), 0.5f, 0.5f, 0.5f);

			glUniform1f(glGetUniformLocation(shader->ID, "point_light.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(shader->ID, "point_light.linear"), 0.007);
			glUniform1f(glGetUniformLocation(shader->ID, "point_light.quadratic"), 0.0002);
	
			//glUniform3f(glGetUniformLocation(shader->ID, "dir_light.direction"), -0.2f, -1.0f, -50.3f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.ambient"), 0.45f, 0.45f, 0.45f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.diffuse"), 0.15f, 0.15f, 0.15f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.specular"), 0.1f, 0.1f, 0.1f);

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

			if (mesh->hasEBO)
			{
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
			}

			glBindVertexArray(0);


		}
		

		/*if (debugMode)
		{
			std::shared_ptr<Collider> collider = m_Entities[i]->GetComponent<Collider>();

			if (collider != nullptr && collider->enabled == true)
			{
				DrawColliders(collider, trns);
			}
		}*/
	}
	//    // --------------------
		//debugDepthQuad->use();
		//debugDepthQuad->setFloat("near_plane", near_plane);
		//debugDepthQuad->setFloat("far_plane", far_plane);
		//debugDepthQuad->setInt("depthMap", 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//for (int i = 0; i < m_Entities.size(); i++)
		//{
		//	if (!m_Entities[i]->isActive)
		//		continue;

		//	std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		//	std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();

		//	if (mainCamera->GetComponent<Camera>()->DistanceFromCameraTarget(trns) > 14.0f)
		//	{
		//		continue;
		//	}

		//	modelsDrawnCount++;
		//	debugDepthQuad->setMat4("model", trns->GetGlobalMatrix());
		//	if (mesh != nullptr)
		//	{
		//		unsigned int diffuseNr = 1;
		//		unsigned int specularNr = 1;
		//		int anyTexture = 0;


		//		// render scene from light's point of view

		//		
		//		


		//		glBindVertexArray(mesh->GetVAO());

		//		if (mesh->hasEBO)
		//		{
		//			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		//		}
		//		else
		//		{
		//			glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
		//		}

		//		glBindVertexArray(0);





		//	}
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//std::cout << "Models drawn: " << modelsDrawnCount << "\n";
}
void Renderer::DrawGrass() const
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glm::mat4 view = camera.GetViewMatrix();
	instanceManagers[0]->m_shader->use();
	instanceManagers[0]->m_shader->setMat4("projection", mainCamera->GetComponent<Camera>()->GetProjection());
	instanceManagers[0]->m_shader->setMat4("view", mainCamera->GetComponent<Transform>()->GetGlobalMatrix());
	instanceManagers[0]->m_shader->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, instanceManagers[0]->m_mesh->material->GetTextures()[0].id);

	glBindVertexArray(instanceManagers[0]->m_model->meshes[0].GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, instanceManagers[0]->m_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, instanceManagers[0]->amount);
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, instanceManagers.at[0]->m_model.get

}
void Renderer::RegisterManager(std::shared_ptr<InstanceManager> instanceManager)
{
	instanceManagers.push_back(instanceManager);
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
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0, 1.0f, 0.0f)));

	unsigned int hasTexture = glGetUniformLocation(shader->ID, "hasTexture");
	glUniform1i(hasTexture, 0);

	glDrawArrays(GL_LINE_STRIP, 0, col->polygon.shape.size() + 1);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Renderer::DrawFrustum(Frustum frustum) const
{
	Shader* shader = defaultShader;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	int pointsCount = 6 * 3 * 3;

	float* vertices = new float[pointsCount];

	vertices[0] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].x;
	vertices[1] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].y;
	vertices[2] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].z;
	vertices[3] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].x;
	vertices[4] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].y;
	vertices[5] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].z;
	vertices[6] = frustum.fnear[Frustum::COORD_TOPLEFT].x;
	vertices[7] = frustum.fnear[Frustum::COORD_TOPLEFT].y;
	vertices[8] = frustum.fnear[Frustum::COORD_TOPLEFT].z;
	vertices[9] = frustum.ffar[Frustum::COORD_TOPLEFT].x;
	vertices[10] = frustum.ffar[Frustum::COORD_TOPLEFT].y;
	vertices[11] = frustum.ffar[Frustum::COORD_TOPLEFT].z;
	vertices[12] = frustum.ffar[Frustum::COORD_TOPRIGHT].x;
	vertices[13] = frustum.ffar[Frustum::COORD_TOPRIGHT].y;
	vertices[14] = frustum.ffar[Frustum::COORD_TOPRIGHT].z;
	vertices[15] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].x;
	vertices[16] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].y;
	vertices[17] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].z;
	vertices[18] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].x;
	vertices[19] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].y;
	vertices[20] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].z;
	vertices[21] = frustum.fnear[Frustum::COORD_TOPLEFT].x;
	vertices[22] = frustum.fnear[Frustum::COORD_TOPLEFT].y;
	vertices[23] = frustum.fnear[Frustum::COORD_TOPLEFT].z;
	vertices[24] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].x;
	vertices[25] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].y;
	vertices[26] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].z;
	vertices[27] = frustum.ffar[Frustum::COORD_TOPRIGHT].x;
	vertices[28] = frustum.ffar[Frustum::COORD_TOPRIGHT].y;
	vertices[29] = frustum.ffar[Frustum::COORD_TOPRIGHT].z;
	vertices[30] = frustum.fnear[Frustum::COORD_TOPRIGHT].x;
	vertices[31] = frustum.fnear[Frustum::COORD_TOPRIGHT].y;
	vertices[32] = frustum.fnear[Frustum::COORD_TOPRIGHT].z;
	vertices[33] = frustum.ffar[Frustum::COORD_BOTTOMRIGHT].x;
	vertices[34] = frustum.ffar[Frustum::COORD_BOTTOMRIGHT].y;
	vertices[35] = frustum.ffar[Frustum::COORD_BOTTOMRIGHT].z;
	vertices[36] = frustum.ffar[Frustum::COORD_TOPLEFT].x;
	vertices[37] = frustum.ffar[Frustum::COORD_TOPLEFT].y;
	vertices[38] = frustum.ffar[Frustum::COORD_TOPLEFT].z;
	vertices[39] = frustum.ffar[Frustum::COORD_TOPRIGHT].x;
	vertices[40] = frustum.ffar[Frustum::COORD_TOPRIGHT].y;
	vertices[41] = frustum.ffar[Frustum::COORD_TOPRIGHT].z;
	vertices[42] = frustum.fnear[Frustum::COORD_TOPLEFT].x;
	vertices[43] = frustum.fnear[Frustum::COORD_TOPLEFT].y;
	vertices[44] = frustum.fnear[Frustum::COORD_TOPLEFT].z;
	vertices[45] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].x;
	vertices[46] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].y;
	vertices[47] = frustum.fnear[Frustum::COORD_BOTTOMLEFT].z;
	vertices[48] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].x;
	vertices[49] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].y;
	vertices[50] = frustum.ffar[Frustum::COORD_BOTTOMLEFT].z;
	vertices[51] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].x;
	vertices[52] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].y;
	vertices[53] = frustum.fnear[Frustum::COORD_BOTTOMRIGHT].z;
	
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

	glDrawArrays(GL_LINE_STRIP, 0, 54);

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