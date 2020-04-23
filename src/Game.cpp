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
#include "MapLoader.h"

#include <fstream>
#include <iostream>

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
	
	LoadMap(2,2, renderer, assetManager, physics);

	//float fTheta = glm::pi<float>() * 2.0f / 5.0f;
	std::vector<glm::vec2> colliderShape;
	/*for (int i = 0; i < 5; i++)
	{
		colliderShape.push_back({ 30.0f * glm::cos(fTheta * i), 30.0f * glm::sin(fTheta * i) });
	}*/

	colliderShape.push_back({ -5.0f, -5.0f });
	colliderShape.push_back({ -5.0f, 5.0f });
	colliderShape.push_back({ 5.0f, 5.0f });
	colliderShape.push_back({ 5.0f, -5.0f });

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
	/*character->AddComponent<Material>();
	character->GetComponent<Material>()->SetShader(shader);*/
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
	character2->AddComponent<Material>();
	character2->GetComponent<Material>()->SetShader(shader);
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

	//nanosuit 2
	std::shared_ptr<Entity> character3 = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
	character3->AddComponent<Transform>();
	character3->GetComponent<Transform>()->SetPosition(glm::vec3(-1.5f, 0.0f, -1.0f));
	character3->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character3->AddComponent<Mesh>();
	character3->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character3->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character3->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character3->GetComponent<Mesh>()->setupMesh();
	character3->AddComponent<Material>();
	character3->GetComponent<Material>()->SetShader(shader);
	colliderShape.clear();
	colliderShape.push_back({ -5.0f, -3.0f });
	colliderShape.push_back({ -3.0f, -5.0f });
	colliderShape.push_back({ 3.0f, -5.0f });
	colliderShape.push_back({ 5.0f, -3.0f });
	colliderShape.push_back({ 5.0f, 3.0f });
	colliderShape.push_back({ 3.0f,  5.0f });
	colliderShape.push_back({ -3.0f, 5.0f });
	colliderShape.push_back({ -5.0f, 3.0f });

	character3->AddComponent<Collider>();
	character3->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character3);
	renderer->RegisterEntity(character3);


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

		
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_A))
		{
			std::cout << "LOG :: Hold A\n";
			//character->GetComponent<Transform>()->Move(Transform::Right() * (float)Time::GetDeltaTime() * 300.0f);
			character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * 50.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_D))
		{
			std::cout << "LOG :: Hold D\n";
			//character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 300.0f);
			character->GetComponent<Transform>()->Rotate(Transform::Up() * (float)Time::GetDeltaTime() * -50.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_W))
		{
			std::cout << "LOG :: Hold W\n";
			character->GetComponent<Transform>()->Move(Transform::Forward() * (float)Time::GetDeltaTime() * 25.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_S))
		{
			std::cout << "LOG :: Hold S\n";
			character->GetComponent<Transform>()->Move(Transform::Back() * (float)Time::GetDeltaTime() * 25.0f);
		}

		//camera movement
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_J))
		{
			std::cout << "LOG :: Hold J\n";
			//character->GetComponent<Transform>()->Move(Transform::Right() * (float)Time::GetDeltaTime() * 300.0f);
			camera->GetComponent<Transform>()->Move(Transform::Right() * (float)Time::GetDeltaTime() * 20.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_L))
		{
			std::cout << "LOG :: Hold L\n";
			//character->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 300.0f);
			camera->GetComponent<Transform>()->Move(Transform::Left() * (float)Time::GetDeltaTime() * 20.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_I))
		{
			std::cout << "LOG :: Hold I\n";
			camera->GetComponent<Transform>()->Move(Transform::Forward() * (float)Time::GetDeltaTime() * 20.0f);
		}
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_K))
		{
			std::cout << "LOG :: Hold K\n";
			camera->GetComponent<Transform>()->Move(Transform::Back() * (float)Time::GetDeltaTime() * 20.0f);
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
void Game::LoadMap(int sizeX, int sizeY, Renderer* renderer, AssetManager* assetManager, Physics* physics)
{
	std::vector<std::shared_ptr<Entity>> map;
	Model* testModel = assetManager->LoadModel("./res/models/nanosuit/nanosuit.obj");
	for (int x = 0; x < sizeX; x++)
	{
		{
			for (int j = 0; j < sizeY; j++)
			{
				std::ifstream file;
				file.open("./res/maps/Map1.txt");
				if (!file)
				{
					std::cout << "Unable to open file txt";
				}
				float x;
				std::vector<float> temps;
				while (file >> x)
				{
					temps.push_back(x);
					
				}
				
				std::shared_ptr<Entity> chunk = m_EntityManager.CreateEntity<Entity>(&m_ComponentManager);
				chunk->AddComponent<Transform>();
				chunk->GetComponent<Transform>()->SetPosition(glm::vec3(temps[0]+x, temps[1], temps[2]+j));
				std::cout << temps[0] << " " << temps[1] << " " << temps[2];
				chunk->GetComponent<Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

				chunk->AddComponent<Mesh>();
				chunk->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
				chunk->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
				chunk->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
				chunk->GetComponent<Mesh>()->setupMesh();

				map.push_back(chunk);
				renderer->RegisterEntity(chunk);
				physics->RegisterEntity(chunk);
				temps.clear();

			}
		}
	}
	for (int i = 0; i < map.size(); i++)
	{
		std::cout << map[i]->GetEntityID() << std::endl;
	}
}