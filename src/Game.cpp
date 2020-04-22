#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "Player.h"
#include "Time.h"
#include "InputHandler.h"
#include "InputSystem.h"
#include "Collider.h"


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

	//float fTheta = glm::pi<float>() * 2.0f / 5.0f;
	std::vector<glm::vec2> colliderShape;
	/*for (int i = 0; i < 5; i++)
	{
		colliderShape.push_back({ 30.0f * glm::cos(fTheta * i), 30.0f * glm::sin(fTheta * i) });
	}*/

	/*colliderShape.push_back({ -5.0f, -5.0f });
	colliderShape.push_back({ -5.0f, 5.0f });
	colliderShape.push_back({ 5.0f, 5.0f });
	colliderShape.push_back({ 5.0f, -5.0f });*/
	colliderShape.clear();
	colliderShape.push_back({ -5.0f, -3.0f });
	colliderShape.push_back({ -3.0f, -5.0f });
	colliderShape.push_back({ 3.0f, -5.0f });
	colliderShape.push_back({ 5.0f, -3.0f });
	colliderShape.push_back({ 5.0f, 3.0f });
	colliderShape.push_back({ 3.0f,  5.0f });
	colliderShape.push_back({ -3.0f, 5.0f });
	colliderShape.push_back({ -5.0f, 3.0f });

	//Object with model
	std::shared_ptr<Entity> character = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character->AddComponent<Transform>();
	character->GetComponent<Transform>()->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	character->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character->AddComponent<Mesh>();
	character->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character->GetComponent<Mesh>()->setupMesh();

	character->AddComponent<Collider>();
	character->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//nanosuit 2
	std::shared_ptr<Entity> character2 = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character2->AddComponent<Transform>();
	character2->GetComponent<Transform>()->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	character2->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character2->AddComponent<Mesh>();
	character2->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character2->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character2->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character2->GetComponent<Mesh>()->setupMesh();

	colliderShape.clear();
	colliderShape.push_back({ -5.0f, -3.0f });
	colliderShape.push_back({ -3.0f, -5.0f });
	colliderShape.push_back({ 3.0f, -5.0f });
	colliderShape.push_back({ 5.0f, -3.0f });
	colliderShape.push_back({ 5.0f, 3.0f });
	colliderShape.push_back({ 3.0f,  5.0f });
	colliderShape.push_back({ -3.0f, 5.0f });
	colliderShape.push_back({ -5.0f, 3.0f });

	character2->AddComponent<Collider>();
	character2->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character2);
	renderer->RegisterEntity(character2);

	//wall
	std::shared_ptr<Entity> wall = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	wall->AddComponent<Transform>();
	wall->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	colliderShape.clear();
	colliderShape.push_back({ -10.0f, -1.5f });
	colliderShape.push_back({ -10.0f, 1.5f });
	colliderShape.push_back({ 10.0f, 1.5f });
	colliderShape.push_back({ 10.0f, -1.5f });

	wall->AddComponent<Collider>();
	wall->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(wall);
	renderer->RegisterEntity(wall);


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
	camera->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 2.0f, -6.0f));
	camera->GetComponent<Transform>()->SetRotation(glm::vec3(45.0f, 0.0f, 0.0f));

	renderer->SetCamera(camera);
	renderer->debugMode = true;

	//---------------------------------------------------------------------------------

	while (true)
	{
		//physics->FixedUpdate();
		Time::RunTimer();
		glfwPollEvents();

		//input must be early to read from it
		inputSystem->Update();

		if (inputHandler->GetComponent<InputHandler>()->GetKeyDown(GLFW_KEY_A))
		{
			//std::cout << "LOG :: Pressed D\n";
			character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 50.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyUp(GLFW_KEY_A))
		{
			//std::cout << "LOG :: Released D\n";
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_A))
		{
			//std::cout << "LOG :: Hold D\n";
			//character->GetComponent<Transform>()->Move(Transform::Right() * (float)Time::GetDeltaTime() * 300.0f);
			character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * 500.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_D))
		{
			//std::cout << "LOG :: Hold D\n";
			//character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 300.0f);
			character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * (-500.0f));
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_W))
		{
			//std::cout << "LOG :: Hold D\n";
			character->GetComponent<Transform>()->Move(Transform::Forward() * (float)Time::GetDeltaTime() * 300.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_S))
		{
			//std::cout << "LOG :: Hold D\n";
			character->GetComponent<Transform>()->Move(Transform::Back() * (float)Time::GetDeltaTime() * 300.0f);
		}

		//cube object 
		cube->GetComponent<Material>()->SetColor(glm::vec3(sin((GLfloat)glfwGetTime()), 1.0f, cos((GLfloat)glfwGetTime() * 0.24f)));
		cube->GetComponent<Transform>()->SetRotation(glm::vec3(0.0f, (GLfloat)glfwGetTime() * 5.0f, (GLfloat)glfwGetTime() * 10.0f));

		//character object
		character->GetComponent<Mesh>()->material->SetColor(glm::vec3(1.0f, sin((GLfloat)glfwGetTime()), sin((GLfloat)glfwGetTime())));
		//character->GetComponent<Transform>()->SetRotation(glm::vec3((GLfloat)glfwGetTime() * (-5.0f), 0.0f, (GLfloat)glfwGetTime() * 2.0f));
		//scene graph required!!!
		//character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * 5.0f);
		//character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 300.0f);

		physics->Update();
		renderer->Update();
		renderer->DrawCube(cube->GetComponent<Transform>(), cube->GetComponent<Material>());

		physics->LateUpdate();
		renderer->LateUpdate();
		inputSystem->LateUpdate();
	}
}