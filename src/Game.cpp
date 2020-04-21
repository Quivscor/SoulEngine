#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "Player.h"
#include "Time.h"
#include "ColliderMesh.h"
#include "ColliderElipse.h"
#include "InputHandler.h"
#include "InputSystem.h"


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
	InputSystem* inputSystem = new InputSystem();

	Model* testModel = assetManager->LoadModel("./res/models/nanosuit/nanosuit.obj");

	//Object with model
	std::shared_ptr<Entity> character = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character->AddComponent<Transform>();
	character->GetComponent<Transform>()->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	character->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character->AddComponent<ColliderElipse>();

	//y value works like shit. If its less than 0.9f collisions don't work wtf
	character->GetComponent<ColliderElipse>()->radius = glm::vec3(0.15f, 1.0f, 0.15f);

	character->AddComponent<Mesh>();
	character->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character->GetComponent<Mesh>()->setupMesh();

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//nanosuit 2
	ColliderMesh* testCollider = assetManager->LoadCollider("./res/models/nanosuit/nanoSuitCollider.obj");
	std::shared_ptr<Entity> character2 = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character2->AddComponent<Transform>();
	character2->GetComponent<Transform>()->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	character2->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character2->AddComponent<Mesh>();
	character2->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character2->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character2->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character2->GetComponent<Mesh>()->setupMesh();

	character2->AddComponent<ColliderMesh>();
	character2->GetComponent<ColliderMesh>()->originVertices = testCollider->originVertices;
	character2->GetComponent<ColliderMesh>()->vertices = std::vector<glm::vec3>(testCollider->originVertices);
	character2->GetComponent<ColliderMesh>()->indices = testCollider->indices;

	physics->RegisterEntity(character2);
	renderer->RegisterEntity(character2);


	//Camera object
	std::shared_ptr<Entity> camera = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	
	camera->AddComponent<Transform>();
	physics->RegisterEntity(camera);
	camera->AddComponent<Camera>();

	//Input register test
	std::shared_ptr<Entity> inputHandler = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	inputHandler->AddComponent<InputHandler>();
	inputSystem->RegisterEntity(inputHandler);

	//Object for cube
	std::shared_ptr<Entity> cube = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	cube->AddComponent<Transform>();
	cube->AddComponent<Material>();
	cube->GetComponent<Material>()->SetShader(shader);

	cube->GetComponent<Transform>()->SetPosition(glm::vec3(1.0f, -1.0f, -2.0f));
	cube->GetComponent<Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));
	cube->GetComponent<Transform>()->SetScale(glm::vec3(0.5f, 0.75f, 1.0f));

	physics->RegisterEntity(cube);

	//set camera position
	camera->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, -6.0f));

	renderer->SetCamera(camera);

	//---------------------------------------------------------------------------------

	while (true)
	{
		physics->FixedUpdate();
		Time::RunTimer();
		glfwPollEvents();

		//input must be early to read from it
		inputSystem->Update();

		if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_D))
		{
			std::cout << "LOG :: Pressed D\n";
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyUp(GLFW_KEY_D))
		{
			std::cout << "LOG :: Released D\n";
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_D))
		{
			std::cout << "LOG :: Hold D\n";
		}

		//cube object 
		cube->GetComponent<Material>()->SetColor(glm::vec3(sin((GLfloat)glfwGetTime()), 1.0f, cos((GLfloat)glfwGetTime() * 0.24f)));
		cube->GetComponent<Transform>()->SetRotation(glm::vec3(0.0f, (GLfloat)glfwGetTime() * 5.0f, (GLfloat)glfwGetTime() * 10.0f));

		//character object
		character->GetComponent<Mesh>()->material->SetColor(glm::vec3(1.0f, sin((GLfloat)glfwGetTime()), sin((GLfloat)glfwGetTime())));
		//character->GetComponent<Transform>()->SetRotation(glm::vec3((GLfloat)glfwGetTime() * (-5.0f), 0.0f, (GLfloat)glfwGetTime() * 2.0f));

		//scene graph required!!!
		//character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * 5.0f);

		character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 2.0f);

		physics->Update();
		renderer->Update();
		renderer->DrawCube(cube->GetComponent<Transform>(), cube->GetComponent<Material>());

		physics->LateUpdate();
		renderer->LateUpdate();
		inputSystem->LateUpdate();
	}
}