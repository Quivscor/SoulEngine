#include "Renderer.h"
 #include "Model.h"
#include "TextRendering.h"
#include<Billboard.h>
#include <stb_image.h>
Shader* Renderer::defaultShader = nullptr;
Shader* Renderer::screenShader = nullptr;

Renderer::Renderer(Shader* shader, Shader* screenShader, Shader* skyBoxShader, Shader* refractorShader, Model* crystal)
{
	lightPos = glm::vec3(50.0f,30.0f, 60.f);
	this->lightProjection = glm::mat4(glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, near_plane, far_plane));
	
	//this->lightView = glm::lookAt(lightPos, glm::vec3(0.0f), mainCamera->GetComponent<Camera>()->upVector);
	this->lightView = glm::lookAt(lightPos, glm::vec3(50.0f, 0.0f, 40.0f), glm::vec3(.0, -1.0,-1.0));

	this->lightSpaceMatrix = lightProjection * lightView;
	defaultShader = shader;
	this->screenShader = screenShader;
	this->skyBoxShader = skyBoxShader;
	this->refractorShader = refractorShader;
	this->crystal = crystal;
	/*box = new Billboard("./res/textures/ExampleBillboard.DDS", true);
	box2 = new Billboard("./res/textures/stone.jpg", false);*/
	simpleDepthShader = new Shader("./res/shaders/shadow_mapping_depth.vs", "./res/shaders/shadow_mapping_depth.fs");
	//hud = new HUD(2.f,2.f, -1.f,- 1.f, "./res/textures/unknown1.jpg");
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


	//framebuffer configuration
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//create a color attachment texture
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	//create a renderebuffer object for depth and stencil attachment 
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// skybox VAO
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	std::vector<std::string> faces
	{
		"./res/textures/skybox/right.jpg",
		"./res/textures/skybox/left.jpg",
		"./res/textures/skybox/top.jpg",
		"./res/textures/skybox/bottom.jpg",
		"./res/textures/skybox/front.jpg",
		"./res/textures/skybox/back.jpg",
	};
	cubemapTexture = loadCubemap(faces);
	skyBoxShader->use();
	skyBoxShader->setInt("skybox", 0);

}

Renderer::~Renderer()
{

}

void Renderer::Init()
{
	cameraComponent = mainCamera->GetComponent<Camera>();
	cameraTransform = mainCamera->GetComponent<Transform>();

	std::cout << "\n=== Sorting Renderer Entities by shader\n";
	SortEntities();
}

void Renderer::Update() const
{
	glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	DrawShadows();
	DrawMeshes();
	
	DrawGrass();

	//DrawFrustum(mainCamera->GetComponent<Camera>()->m_Frustum);
	/*glm::mat4 text_matrix_2D = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
	glm::mat4 translate_2d_text = glm::translate(text_matrix_2D, glm::vec3(20.0f, 65.0f, .0f));
	glm::mat4 scale_2d_text = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
	TextRendering::Instance()->draw("Poruszanie - WASD", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_2D);*/
	
	
	std::shared_ptr<Transform> trns = m_Entities[0]->GetComponent<Transform>();
	
	//std::cout << trns->GetLocalPosition().x << std::endl;

	/*Billboard::Instance("./res/textures/ExampleBillboard.DDS", true,mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 1.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
	Billboard::Instance("./res/textures/stone.jpg", false, mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 2.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));*/
	/*Billboard::Instance("./res/textures/ExampleBillboard.DDS", true)->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 1.5f, trns->GetLocalPosition().z - 0.f),glm::vec2(1.0f,0.125f));

	Billboard::Instance("./res/textures/stone.jpg", false)->Draw(mainCamera, glm::vec3(trns->GetLocalPosition().x, trns->GetLocalPosition().y + 2.5f, trns->GetLocalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));*/
	glm::mat4 scale = glm::scale(trns->GetLocalMatrix(), glm::vec3(0.5f, 0.5f, 0.5f));
	//glm::mat4 set_text_to_origin = glm::translate(trns->GetLocalMatrix(), glm::vec3(-2.f, 20.0f, 0.0f));

	glm::mat4 text_rotate_y = glm::rotate(glm::mat4(), glm::radians(-trns->GetLocalRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 text_translate_to_model_1 = glm::translate(trns->GetLocalMatrix(), glm::vec3(trns->GetLocalPosition().x - 2.f, trns->GetLocalPosition().y + 20.0f, trns->GetLocalPosition().z));

	glm::mat4 text_matrix_3D_model_1 = cameraComponent->GetProjection() * cameraTransform->GetGlobalMatrix() * text_translate_to_model_1 * scale;

	//TextRendering::Instance()->draw("Gracz", glm::vec3(1.0f, 0.0f, 0.0f), text_matrix_3D_model_1);

	// drawing crystal object
	refractorShader->use();
	refractorShader->setMat4("projection", cameraComponent->GetProjection());
	refractorShader->setMat4("view", cameraTransform->GetGlobalMatrix());
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(3.0f, 5.0f, 1.5f));
	model = glm::scale(model, glm::vec3(1.0f, 3.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	refractorShader->setMat4("model", model);
	refractorShader->setVec3("cameraPos", cameraTransform->GetGlobalPosition());

	refractorShader->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, instanceManagers[0]->m_mesh->material->GetTextures()[0].id);

	glBindVertexArray(crystal->meshes[0].GetVAO());
	glDrawElements(GL_TRIANGLES, crystal->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// draw skybox last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyBoxShader->use();
	model = glm::mat4(1.0f);
	glm::mat4 view = cameraTransform->GetGlobalMatrix();
	view = glm::mat4(glm::mat3(cameraTransform->GetGlobalMatrix())); // remove translation from the view matrix
	skyBoxShader->setMat4("view", view);
	skyBoxShader->setMat4("model", model);
	glm::mat4 projection = cameraComponent->GetProjection();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	skyBoxShader->setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	DrawHPbar();
	 //Drawparts();
	

	if (berserkerModeActive == true)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		screenShader->use();
		screenShader->setInt("screenTexture", 0);
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	DrawGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//hud->Drawbar();
}

void Renderer::LateUpdate() const
{
	glfwSwapBuffers(Window::GetInstance()->GetMWindow());
}

void Renderer::DrawGUI() const
{
	

	glm::mat4 text_matrix_2D = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);

	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		if (m_Entities[i]->GetComponent<Text>() != nullptr)
		{
			std::shared_ptr<Text> text = m_Entities[i]->GetComponent<Text>();

			TextRendering::Instance()->draw(text->text, text->color, text_matrix_2D * EntityManager::GetInstance()->GetEntity(text->GetOwnerID())->GetComponent<Transform>()->GetGlobalMatrix());
		}
	
	}
	for (int i = 0; i < HUDs.size(); i++)
	{
		if (EntityManager::GetInstance()->GetEntity(HUDs[i].first->GetOwnerID())->isActive == true)
			HUDs[i].first->Drawbar();
	}

}

void Renderer::DrawHPbar() const
{
	for (int i = 0; i < billboards.size(); i++)
	{
		if (EntityManager::GetInstance()->GetEntity(billboards[i].first->GetOwnerID())->isActive == true)
			billboards[i].first->Draw(mainCamera, glm::vec3(billboards[i].second->GetGlobalPosition().x, billboards[i].second->GetGlobalPosition().y + 1.5f, billboards[i].second->GetGlobalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
	}

}

void Renderer::DrawShadows() const
{
	
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene



	simpleDepthShader->use();
	simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);	
	//glActiveTexture(GL_TEXTURE0);
	////glBindTexture(GL_TEXTURE_2D, instanceManagers[0]->m_mesh->material->GetTextures()[0].id);
	//glBindTexture(GL_TEXTURE_2D, cubemapTexture);
	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();
		float renderingRange = 20.0f;
		if (m_Entities[i]->layer == Layer::GroundLayer)
		{
			renderingRange = 36.0f;
		}
		if (m_Entities[i]->layer != Layer::WaterLayer)
		{
			if (cameraComponent->DistanceFromCameraTarget(trns) > renderingRange)
				continue;
		}

		simpleDepthShader->setMat4("model", trns->GetGlobalMatrix());
		//simpleDepthShader->setMat4("model", trns->GetLocalMatrix());
		if (mesh != nullptr)
		{
			std::shared_ptr<Model> modelComponent = m_Entities[i]->GetComponent<Model>();

			if (modelComponent != nullptr)
			{
				modelComponent->initShaders(simpleDepthShader);
				modelComponent->ChangeShadowBonesPositions();

				simpleDepthShader->setBool("anim", true);

				//m_Entities[i]->GetComponent<Mesh>()->material->SetShader(defaultShader);
			}
			else
			{
				simpleDepthShader->setBool("anim", false);
			}
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
	//simpleDepthShader->setMat4("model", *instanceManagers[0]->instanceModels);



	//glBindVertexArray(instanceManagers[0]->m_model->meshes[0].GetVAO());
	//glDrawElementsInstanced(GL_TRIANGLES, instanceManagers[0]->m_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, instanceManagers[0]->amount);
	////glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, 1920, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::DrawMeshes() const
{
	//glm::mat4 lightProjection = glm::mat4(), lightView = glm::mat4();
	//glm::mat4 lightSpaceMatrix = glm::mat4();
	////lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	//lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	////lightView = glm::lookAt(lightPos, glm::vec3(0.0f), mainCamera->GetComponent<Camera>()->upVector);
	//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, -1.0, 0.0));

	//lightSpaceMatrix = lightProjection * lightView;

	int modelsDrawnCount = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (berserkerModeActive == true)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.3f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	for (int i = 0; i < m_Entities.size(); i++)
	{
		if (!m_Entities[i]->isActive)
			continue;

		std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
		std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();

		float renderingRange = 20.0f;
		if (m_Entities[i]->layer == Layer::GroundLayer)
		{
			renderingRange = 36.0f;
		}
		if (m_Entities[i]->layer != Layer::WaterLayer)
		{
			if (cameraComponent->DistanceFromCameraTarget(trns) > renderingRange)
				continue;
		}
			
		modelsDrawnCount++;

		if (mesh != nullptr)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			int anyTexture = 0;

			Shader* shader = m_Entities[i]->GetComponent<Mesh>()->material->GetShader();
			for (int j = 0; j < mesh->material->GetTextures().size(); j++)
			{		
				anyTexture = 1;
				//defaultShader->setInt("material.diffuse", i);
				glActiveTexture(GL_TEXTURE0 + j); //glActiveTexture(diffuse_textureN), where N = GL_TEXTURE0 + i

				std::string number = "";
				std::string name = mesh->material->GetTextures()[j].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				//defaultShader->setFloat(("material." + name + number).c_str(), i);

				glBindTexture(GL_TEXTURE_2D, mesh->material->GetTextures()[j].id);
				//glUniform1i(glGetUniformLocation(shader->ID, ("material." + name + number).c_str()), j);
			}

			
			glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, depthMap);
			/*		glUniform1i(glGetUniformLocation(shader->ID, "material.shadowMap"), mesh->material->GetTextures().size());

					unsigned int xd = glGetUniformLocation(shader->ID, "lightSpaceMatrix");
					glUniformMatrix4fv(xd, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));*/
			
					glm::vec3 playerPosition = mainCamera->GetComponent<Camera>()->cameraTarget->GetGlobalPosition();

			shader->use();
			shader->setVec3("dir_light.direction", glm::vec3(.0, -1.0, -1.0));
			shader->setInt("material.shadowMap", 2);
			shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
			shader->setVec3("playerPosition", playerPosition);
			std::shared_ptr<Model> modelComponent = m_Entities[i]->GetComponent<Model>();

			if (modelComponent != nullptr)
			{
				modelComponent->initShaders(shader);
				modelComponent->ChangeBonePositions();
				
					
				//m_Entities[i]->GetComponent<Mesh>()->material->SetShader(defaultShader);
			}
			glUniform3f(glGetUniformLocation(shader->ID, "view_pos"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);
			glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), 0.5f);
			glUniform1f(glGetUniformLocation(shader->ID, "material.transparency"), 1.0f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.ambient"), 0.45f, 0.45f, 0.45f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.diffuse"), 0.15f, 0.15f, 0.15f);
			glUniform3f(glGetUniformLocation(shader->ID, "dir_light.specular"), 0.1f, 0.1f, 0.1f);
			//// Point Light 1
			//glUniform3f(glGetUniformLocation(shader->ID, "point_light.position"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);

			//glUniform3f(glGetUniformLocation(shader->ID, "point_light.ambient"),  -0.2f, -1.0f, -0.3f);
			//glUniform3f(glGetUniformLocation(shader->ID, "point_light.diffuse"), 0.4f, 0.4f, 0.4f);
			//glUniform3f(glGetUniformLocation(shader->ID, "point_light.specular"), 0.5f, 0.5f, 0.5f);

			//glUniform1f(glGetUniformLocation(shader->ID, "point_light.constant"), 1.0f);
			//glUniform1f(glGetUniformLocation(shader->ID, "point_light.linear"), 0.007);
			//glUniform1f(glGetUniformLocation(shader->ID, "point_light.quadratic"), 0.0002);
	
			////glUniform3f(glGetUniformLocation(shader->ID, "dir_light.direction"), -0.2f, -1.0f, -50.3f);
			//glUniform3f(glGetUniformLocation(shader->ID, "dir_light.ambient"), 0.55f, 0.55f, 0.55f);
			//glUniform3f(glGetUniformLocation(shader->ID, "dir_light.diffuse"), 0.15f, 0.15f, 0.15f);
			//glUniform3f(glGetUniformLocation(shader->ID, "dir_light.specular"), 0.1f, 0.1f, 0.1f);

			//player position
			//glUniform3f(glGetUniformLocation(shader->ID, "playerPosition"), playerPosition.x, playerPosition.y, playerPosition.z);

			glm::mat4 mvp = cameraComponent->GetProjection() * cameraTransform->GetGlobalMatrix() * trns->GetGlobalMatrix();
			/*std::cout << "\n"<< cameraComponent->GetProjection()[0][0] << " " << cameraComponent->GetProjection()[0][1] << " " << cameraComponent->GetProjection()[0][2] << " " << cameraComponent->GetProjection()[0][3]
				<< "\n " << cameraComponent->GetProjection()[1][0] << " " << cameraComponent->GetProjection()[1][1] << " " << cameraComponent->GetProjection()[1][2] << " " << cameraComponent->GetProjection()[1][3]
				<< "\n " << cameraComponent->GetProjection()[2][0] << " " << cameraComponent->GetProjection()[2][1] << " " << cameraComponent->GetProjection()[2][2] << " " << cameraComponent->GetProjection()[2][3]
				<< "\n " << cameraComponent->GetProjection()[3][0] << " " << cameraComponent->GetProjection()[3][1] << " " << cameraComponent->GetProjection()[3][2] << " " << cameraComponent->GetProjection()[3][3];
			std::cout << "\n" << cameraTransform->GetGlobalMatrix()[0][0] << " " << cameraTransform->GetGlobalMatrix()[0][1] << " " << cameraTransform->GetGlobalMatrix()[0][2] << " " << cameraTransform->GetGlobalMatrix()[0][3]
				<< "\n " << cameraTransform->GetGlobalMatrix()[1][0] << " " << cameraTransform->GetGlobalMatrix()[1][1] << " " << cameraTransform->GetGlobalMatrix()[1][2] << " " << cameraTransform->GetGlobalMatrix()[1][3]
				<< "\n " << cameraTransform->GetGlobalMatrix()[2][0] << " " << cameraTransform->GetGlobalMatrix()[2][1] << " " << cameraTransform->GetGlobalMatrix()[2][2] << " " << cameraTransform->GetGlobalMatrix()[2][3]
				<< "\n " << cameraTransform->GetGlobalMatrix()[3][0] << " " << cameraTransform->GetGlobalMatrix()[3][1] << " " << cameraTransform->GetGlobalMatrix()[3][2] << " " << cameraTransform->GetGlobalMatrix()[3][3];
			std::cout << "\n" << trns->GetGlobalMatrix()[0][0] << " " << trns->GetGlobalMatrix()[0][1] << " " << trns->GetGlobalMatrix()[0][2] << " " << trns->GetGlobalMatrix()[0][3]
				<< "\n " << trns->GetGlobalMatrix()[1][0] << " " << trns->GetGlobalMatrix()[1][1] << " " << trns->GetGlobalMatrix()[1][2] << " " << trns->GetGlobalMatrix()[1][3]
				<< "\n " << trns->GetGlobalMatrix()[2][0] << " " << trns->GetGlobalMatrix()[2][1] << " " << trns->GetGlobalMatrix()[2][2] << " " << trns->GetGlobalMatrix()[2][3]
				<< "\n " << trns->GetGlobalMatrix()[3][0] << " " << trns->GetGlobalMatrix()[3][1] << " " << trns->GetGlobalMatrix()[3][2] << " " << trns->GetGlobalMatrix()[3][3];*/

			unsigned int model = glGetUniformLocation(shader->ID, "M_matrix");
			glUniformMatrix4fv(model, 1, GL_FALSE, &(trns->GetGlobalMatrix())[0][0]);

			unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &(mvp)[0][0]);

			glUniform1f(glGetUniformLocation(shader->ID, "waveTime"), (float)TimeCustom::GetTime() * 0.1f);

			unsigned int colorLoc = glGetUniformLocation(shader->ID, "color");
			glUniform3fv(colorLoc, 1, glm::value_ptr(mesh->material->GetColor()));
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "M_matrix"), 1, GL_FALSE, &(trns->GetGlobalMatrix())[0][0]);
			glm::mat4 matr_normals_cube = glm::mat4(glm::transpose(glm::inverse(trns->GetGlobalMatrix())));
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "normals_matrix"), 1, GL_FALSE, &matr_normals_cube[0][0]);
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
		
		if(debugMode)
		{
			std::shared_ptr<Collider> collider = m_Entities[i]->GetComponent<Collider>();

			if (collider != nullptr && collider->enabled == true)
			{
				DrawColliders(collider, trns);
			}
		}
	}

	

	
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//std::cout << "Models drawn: " << modelsDrawnCount << "\n";
}
void Renderer::DrawGrass() const
{
	//simpleDepthShader->use();
	//simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//*std::shared_ptr<Transform> trns = m_Entities[i]->GetComponent<Transform>();
	//std::shared_ptr<Mesh> mesh = m_Entities[i]->GetComponent<Mesh>();*/

	//simpleDepthShader->setMat4("model",* instanceManagers[0]->instanceModels);



	//glBindVertexArray(instanceManagers[0]->m_model->meshes[0].GetVAO());
	//glDrawElementsInstanced(GL_TRIANGLES, instanceManagers[0]->m_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, instanceManagers[0]->amount);
	//glBindVertexArray(0);



	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// reset viewport
	//glViewport(0, 0, 1280, 720);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 lightProjection = glm::mat4(), lightView = glm::mat4();
	//glm::mat4 lightSpaceMatrix = glm::mat4();
	////lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	//lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	////lightView = glm::lookAt(lightPos, glm::vec3(0.0f), mainCamera->GetComponent<Camera>()->upVector);
	//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, -1.0, 0.0));

	//lightSpaceMatrix = lightProjection * lightView;
	instanceManagers[0]->m_shader->use();
	instanceManagers[0]->m_shader->setVec3("dir_light.direction", glm::vec3(.0, -1.0, -1.0));
	glUniform3f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "view_pos"), mainCamera->GetComponent<Transform>()->GetGlobalPosition().x, mainCamera->GetComponent<Transform>()->GetGlobalPosition().y, mainCamera->GetComponent<Transform>()->GetGlobalPosition().z);
	glUniform1f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "material.shininess"), 0.5f);
	glUniform1f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "material.transparency"), 1.0f);
	glUniform3f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "dir_light.ambient"), 0.45f, 0.45f, 0.45f);
	glUniform3f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "dir_light.diffuse"), 0.15f, 0.15f, 0.15f);
	glUniform3f(glGetUniformLocation(instanceManagers[0]->m_shader->ID, "dir_light.specular"), 0.1f, 0.1f, 0.1f);
	instanceManagers[0]->m_shader->setInt("material.shadowMap", 2);
	instanceManagers[0]->m_shader->setMat4("projection", cameraComponent->GetProjection());
	instanceManagers[0]->m_shader->setMat4("view", cameraTransform->GetGlobalMatrix());
	instanceManagers[0]->m_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	instanceManagers[0]->m_shader->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, instanceManagers[0]->m_mesh->material->GetTextures()[0].id);

	glBindVertexArray(instanceManagers[0]->m_model->meshes[0].GetVAO());
	glDrawElementsInstanced(GL_TRIANGLES, instanceManagers[0]->m_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, instanceManagers[0]->amount);
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, instanceManagers.at[0]->m_model.get







	////glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	////glm::mat4 view = camera.GetViewMatrix();
	//instanceManagers[0]->m_shader->use();
	//instanceManagers[0]->m_shader->setMat4("projection", cameraComponent->GetProjection());
	//instanceManagers[0]->m_shader->setMat4("view", cameraTransform->GetGlobalMatrix());
	//instanceManagers[0]->m_shader->setInt("texture_diffuse1", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, instanceManagers[0]->m_mesh->material->GetTextures()[0].id);

	//glBindVertexArray(instanceManagers[0]->m_model->meshes[0].GetVAO());
	//glDrawElementsInstanced(GL_TRIANGLES, instanceManagers[0]->m_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, instanceManagers[0]->amount);
	//glBindVertexArray(0);
	////glBindTexture(GL_TEXTURE_2D, instanceManagers.at[0]->m_model.get

}
void Renderer::RegisterManager(std::shared_ptr<InstanceManager> instanceManager)
{
	instanceManagers.push_back(instanceManager);
}

void Renderer::Drawparts() const
{
	for (int i = 0; i < Parts.size(); i++)
	{
		if (EntityManager::GetInstance()->GetEntity(Parts[i].first->GetOwnerID())->isActive == true)
			Parts[i].first->Draw(mainCamera, glm::vec3(Parts[i].second->GetGlobalPosition().x, Parts[i].second->GetGlobalPosition().y + 1.5f, Parts[i].second->GetGlobalPosition().z - 0.f), glm::vec2(1.0f, 0.125f));
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
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &(mvp)[0][0]);

	unsigned int colorLoc = glGetUniformLocation(shader->ID, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0, 1.0f, 0.0f)));

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

	glm::mat4 mvp = cameraComponent->GetProjection() * cameraTransform->GetGlobalMatrix() * transform->GetGlobalMatrix();

	unsigned int transformLoc = glGetUniformLocation(defaultShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &(mvp)[0][0]);

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

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Renderer::SortEntities()
{
	std::sort(m_Entities.begin(), m_Entities.end(),
		[](const std::shared_ptr<Entity> &entity1, const std::shared_ptr<Entity> &entity2)
	{
		unsigned int shaderID1 = 0, shaderID2 = 0;
		if (entity1->GetComponent<Mesh>() != nullptr)
		{
			shaderID1 = entity1->GetComponent<Mesh>()->material->GetShader()->ID;
		}
		if (entity2->GetComponent<Mesh>() != nullptr)
		{
			shaderID2 = entity2->GetComponent<Mesh>()->material->GetShader()->ID;
		}
		return shaderID1 > shaderID2;
	});
}

void Renderer::RegisterBillboard(std::shared_ptr<Entity> billboard)
{
	billboards.push_back(std::make_pair(billboard->GetComponent<Billboard>(), billboard->GetComponent<Transform>()));
}

void Renderer::RegisterHUD(std::shared_ptr<Entity> hut)
{
	HUDs.push_back(std::make_pair(hut->GetComponent<HUD>(), hut->GetComponent<Transform>()));
}

void Renderer::RegisterPar(std::shared_ptr<Entity> Part)
{
	Parts.push_back(std::make_pair(Part->GetComponent<Particles>(), Part->GetComponent<Transform>()));
}
