#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "Player.h"

Game::Game() {}

Game::~Game() 
{
	glfwTerminate();
}

void Game::Init()
{
	m_Window = m_Window->GetInstance();
	m_Window->Init("SoulEater", 640, 480);

	LogEvent e("Hello events!");
	m_Window->EventCallback(e);
}

void Game::Run()
{
	//-----------------------------Tests zone------------------------------------------
	//TO DELETE:
	//Creating simple shader
	Shader* shader = new Shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");

	//Creating systems
	AssetManager* assetManager = new AssetManager();
	Renderer* renderer = new Renderer(shader);
	Physics* physics = new Physics();

	Model* testModel = assetManager->LoadModel("./res/models/nanosuit/nanosuit.obj");

	//Object with model
	std::shared_ptr<Entity> character = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character->AddComponent<Transform>();
	character->GetComponent<Transform>()->scale = glm::vec3(0.1f, 0.1f, 0.1f);
	character->AddComponent<Mesh>();

	character->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character->GetComponent<Mesh>()->setupMesh();

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//Camera object
	std::shared_ptr<Entity> camera = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	
	camera->AddComponent<Transform>();
	physics->RegisterEntity(camera);
	camera->AddComponent<Camera>();

	//Object for cube
	std::shared_ptr<Entity> cube = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	cube->AddComponent<Transform>();
	cube->AddComponent<Material>();
	cube->GetComponent<Material>()->SetShader(shader);

	cube->GetComponent<Transform>()->position = glm::vec3(0.33f, -1.0f, 0.0f);
	cube->GetComponent<Transform>()->rotation = glm::vec3(0.0f, 0.0f, 45.0f);
	cube->GetComponent<Transform>()->scale = glm::vec3(0.5f, 0.75f, 1.0f);

	physics->RegisterEntity(cube);

	//set camera position
	camera->GetComponent<Transform>()->position = glm::vec3(0.0f, 0.0f, -3.0f);

	renderer->SetCamera(camera);

	//---------------------------------------------------------------------------------

	while (true)
	{
		//cube object 
		cube->GetComponent<Material>()->SetColor(glm::vec3(sin((GLfloat)glfwGetTime()), 1.0f, cos((GLfloat)glfwGetTime() * 0.24f)));
		cube->GetComponent<Transform>()->rotation = glm::vec3(0.0f, (GLfloat)glfwGetTime() * 5.0f, (GLfloat)glfwGetTime() * 10.0f);

		//character object
		character->GetComponent<Mesh>()->material->SetColor(glm::vec3(1.0f, sin((GLfloat)glfwGetTime()), sin((GLfloat)glfwGetTime())));
		character->GetComponent<Transform>()->rotation = glm::vec3((GLfloat)glfwGetTime() * (-5.0f), 0.0f, (GLfloat)glfwGetTime() * 2.0f);

		physics->Update();
		renderer->Update();
		renderer->DrawCube(cube->GetComponent<Transform>(), cube->GetComponent<Material>());

		physics->LateUpdate();
		renderer->LateUpdate();
	}
}