#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "Time.h"
#include "InputHandler.h"
#include "Collider.h"
#include "MapLoader.h"
#include "Scripts/ColorChanger.h"
#include "Scripts/HelloTriggers.h"
#include "Scripts/CameraFollow.h"
#include "Scripts/Player.h"
#include "Scripts/Weapon.h"

#include <fstream>
#include <iostream>

Game::Game() {}

Game::~Game() 
{
	glfwTerminate();
}

void Game::Init()
{
	m_EntityManager = EntityManager::GetInstance();
	m_ComponentManager = ComponentManager::GetInstance();

	m_Window = m_Window->GetInstance();
	m_Window->Init("SoulEater", 1280, 720);

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
	GameLogic* gameLogic = new GameLogic();

	//Input register test
	std::shared_ptr<Entity> inputHandler = m_EntityManager->CreateEntity<Entity>();
	inputHandler->AddComponent<InputHandler>();
	inputSystem->RegisterEntity(inputHandler);

	EntitiesInit(assetManager, renderer, physics, gameLogic, inputHandler);

	gameLogic->Start();

	//---------------------------------------------------------------------------------

	while (true)
	{
		//physics->FixedUpdate();
		Time::RunTimer();
		//double start = glfwGetTime();
		glfwPollEvents();

		//input must be early to read from it
		inputSystem->Update();

		gameLogic->Update();

		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_ESCAPE)) {
			glfwTerminate();
			glfwSetWindowShouldClose(m_Window->GetMWindow(), true);
			exit(3);
		}

		physics->Update();
		renderer->Update();

		physics->LateUpdate();
		renderer->LateUpdate();
		inputSystem->LateUpdate();
	}
}

void Game::LoadMap(int sizeX, int sizeY, Renderer* renderer, AssetManager* assetManager, Physics* physics)
{
	std::vector<std::shared_ptr<Entity>> map;
	Model* testModel = assetManager->LoadModel("./res/models/chunk.obj");
	for (int x = 0; x < sizeX; x++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			std::ifstream file;
			file.open("./res/maps/Map1.txt");
			if (!file)
			{
				std::cout << "Unable to open file txt";
			}
			float var;
			std::vector<float> temps;
			while (file >> var)
			{
				temps.push_back(var);

			}

			std::shared_ptr<Entity> chunk = m_EntityManager->CreateEntity<Entity>();
			chunk->AddComponent<Transform>();
			chunk->GetComponent<Transform>()->SetLocalPosition(glm::vec3(temps[0] + x*10, temps[1], temps[2] + j*10));
			chunk->GetComponent<Transform>()->SetLocalScale(glm::vec3(temps[3], temps[4], temps[5]));

			chunk->AddComponent<Mesh>();
			chunk->GetComponent<Mesh>()->indices = testModel->GetMeshes()[0].indices;
			chunk->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[0].vertices;
			chunk->GetComponent<Mesh>()->material = testModel->GetMeshes()[0].material;
			chunk->GetComponent<Mesh>()->setupMesh();

			map.push_back(chunk);
			renderer->RegisterEntity(chunk);
			physics->RegisterEntity(chunk);
			temps.clear();

		}
	}
}

void Game::EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem)
{
	Shader* shader = new Shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
	Shader* shadera = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");

	Model* testModel = assetManager->LoadModel("./res/models/nanosuit/nanosuit.obj");
	Model* mapModel = assetManager->LoadModel("./res/models/map/Map1.obj");
	Model* testModela = assetManager->LoadModel("./res/models/man/model.dae");

	//Camera object
	std::shared_ptr<Entity> cameraRoot = m_EntityManager->CreateEntity<Entity>();
	//cameraRoot->GetComponent<Transform>()->SetLocalRotation(glm::vec3(-45.0f, 0, 0));

	cameraRoot->AddComponent<CameraFollow>();
	cameraRoot->GetComponent<Transform>()->DisplayDebugInfo(true);
	physics->RegisterEntity(cameraRoot);
	gameLogic->RegisterEntity(cameraRoot);

	std::shared_ptr<Entity> camera = m_EntityManager->CreateEntity<Entity>();
	camera->AddComponent<Camera>();
	physics->RegisterEntity(camera);
	renderer->SetCamera(camera);
	//renderer->debugMode = true;

	//set camera position
	camera->GetComponent<Transform>()->SetLocalRotation(glm::vec3(45.0f, 0.0f, 0.0f));
	camera->GetComponent<Transform>()->SetParent(cameraRoot->GetComponent<Transform>());
	camera->GetComponent<Transform>()->DisplayDebugInfo(true);
	

	//LoadMap(5,5, renderer, assetManager, physics);
	/*std::cout<<testModela->scene->mAnimations[0]->mDuration;*/
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
	std::shared_ptr<Entity> character = m_EntityManager->CreateEntity<Entity>();
	character->AddComponent<Transform>();
	character->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	character->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));
	character->GetComponent<Transform>()->DisplayDebugInfo(true);

	std::shared_ptr<Entity> map = m_EntityManager->CreateEntity<Entity>();
	map->AddComponent<Transform>();
	map->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	map->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	map->AddComponent<Mesh>();
	map->GetComponent<Mesh>()->indices = mapModel->GetMeshes()[0].indices;
	map->GetComponent<Mesh>()->vertices = mapModel->GetMeshes()[0].vertices;
	map->GetComponent<Mesh>()->material = mapModel->GetMeshes()[0].material;
	map->GetComponent<Mesh>()->setupMesh();
	physics->RegisterEntity(map);
	renderer->RegisterEntity(map);

	std::shared_ptr<Entity> map2 = m_EntityManager->CreateEntity<Entity>();
	map2->AddComponent<Transform>();
	map2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15.0f, 0.0f, 0.0f));
	map2->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	map2->AddComponent<Mesh>();
	map2->GetComponent<Mesh>()->indices = mapModel->GetMeshes()[0].indices;
	map2->GetComponent<Mesh>()->vertices = mapModel->GetMeshes()[0].vertices;
	map2->GetComponent<Mesh>()->material = mapModel->GetMeshes()[0].material;
	map2->GetComponent<Mesh>()->setupMesh();
	physics->RegisterEntity(map2);
	renderer->RegisterEntity(map2);

	std::shared_ptr<Entity> map3 = m_EntityManager->CreateEntity<Entity>();
	map3->AddComponent<Transform>();
	map3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 0.0f, 15.0f));
	map3->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	map3->AddComponent<Mesh>();
	map3->GetComponent<Mesh>()->indices = mapModel->GetMeshes()[0].indices;
	map3->GetComponent<Mesh>()->vertices = mapModel->GetMeshes()[0].vertices;
	map3->GetComponent<Mesh>()->material = mapModel->GetMeshes()[0].material;
	map3->GetComponent<Mesh>()->setupMesh();
	physics->RegisterEntity(map3);
	renderer->RegisterEntity(map3);

	std::shared_ptr<Entity> map4 = m_EntityManager->CreateEntity<Entity>();
	map4->AddComponent<Transform>();
	map4->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 0.0f, 15.0f));
	map4->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

	map4->AddComponent<Mesh>();
	map4->GetComponent<Mesh>()->indices = mapModel->GetMeshes()[0].indices;
	map4->GetComponent<Mesh>()->vertices = mapModel->GetMeshes()[0].vertices;
	map4->GetComponent<Mesh>()->material = mapModel->GetMeshes()[0].material;
	map4->GetComponent<Mesh>()->setupMesh();
	physics->RegisterEntity(map4);
	renderer->RegisterEntity(map4);

	//player
	character->AddComponent<Mesh>();
	character->AddComponent<Model>();

	character->GetComponent<Model>()->UseModel(testModela);
	character->GetComponent<Mesh>()->SetAll(testModela->GetMeshes()[0]);
	character->GetComponent<Mesh>()->material->SetShader(shadera);

	character->GetComponent<Mesh>()->setupMeshfBones();

	character->AddComponent<Player>();
	character->GetComponent<Player>()->inputHandler = inputSystem;

	gameLogic->RegisterEntity(character);

	character->AddComponent<Collider>();
	character->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//player's weapon ! -> it should be spawned in player's script imo
	std::shared_ptr<Entity> weapon = m_EntityManager->CreateEntity<Entity>();
	weapon->AddComponent<Transform>();
	weapon->GetComponent<Transform>()->SetParent(character->GetComponent<Transform>());
	weapon->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 11.0f));
	weapon->AddComponent<Collider>();
	weapon->GetComponent<Collider>()->SetShape(colliderShape);
	weapon->GetComponent<Collider>()->isTrigger = true;
	weapon->AddComponent<Weapon>();

	gameLogic->RegisterEntity(weapon);
	physics->RegisterEntity(weapon);
	renderer->RegisterEntity(weapon);

	character->GetComponent<Player>()->weapon = weapon->GetComponent<Weapon>();

	//nanosuit 2
	std::shared_ptr<Entity> character2 = m_EntityManager->CreateEntity<Entity>();
	character2->AddComponent<Transform>();

	character2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1.0f, 0.0f, 3.0f));
	character2->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

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
	character2->GetComponent<Collider>()->isTrigger = true;
	character2->GetComponent<Collider>()->isStatic = true;
	//character2->AddComponent<HelloTriggers>();
	character2->AddComponent<ColorChanger>();

	physics->RegisterEntity(character2);
	renderer->RegisterEntity(character2);

	//nanosuit 3
	std::shared_ptr<Entity> character3 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	character3->AddComponent<Transform>();

	character3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.5f, 0.0f, 0.5f));
	character3->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character3->AddComponent<Mesh>();
	character3->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character3->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character3->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character3->GetComponent<Mesh>()->setupMesh();
	character3->AddComponent<Material>();
	character3->GetComponent<Material>()->SetShader(shader);
	colliderShape.clear();
	colliderShape.push_back({ -8.0f, -5.0f });
	colliderShape.push_back({ -5.0f, -8.0f });
	colliderShape.push_back({ 5.0f, -8.0f });
	colliderShape.push_back({ 8.0f, -5.0f });
	colliderShape.push_back({ 8.0f, 5.0f });
	colliderShape.push_back({ 5.0f,  8.0f });
	colliderShape.push_back({ -5.0f, 8.0f });
	colliderShape.push_back({ -8.0f, 5.0f });

	character3->AddComponent<Collider>();
	character3->GetComponent<Collider>()->SetShape(colliderShape);
	character3->GetComponent<Collider>()->isStatic = true;

	physics->RegisterEntity(character3);
	renderer->RegisterEntity(character3);

	//nanosuit 4
	std::shared_ptr<Entity> character4 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	character4->AddComponent<Transform>();
	character4->GetComponent<Transform>()->SetLocalPosition(glm::vec3(-0.0f, 0.0f, 3.0f));
	character4->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character4->AddComponent<Mesh>();
	character4->GetComponent<Mesh>()->indices = testModel->GetMeshes()[1].indices;
	character4->GetComponent<Mesh>()->vertices = testModel->GetMeshes()[1].vertices;
	character4->GetComponent<Mesh>()->material = testModel->GetMeshes()[1].material;
	character4->GetComponent<Mesh>()->setupMesh();
	character4->AddComponent<Material>();
	character4->GetComponent<Material>()->SetShader(shader);
	colliderShape.clear();
	colliderShape.push_back({ -8.0f, -5.0f });
	colliderShape.push_back({ -5.0f, -8.0f });
	colliderShape.push_back({ 5.0f, -8.0f });
	colliderShape.push_back({ 8.0f, -5.0f });
	colliderShape.push_back({ 8.0f, 5.0f });
	colliderShape.push_back({ 5.0f,  8.0f });
	colliderShape.push_back({ -5.0f, 8.0f });
	colliderShape.push_back({ -8.0f, 5.0f });

	character4->AddComponent<Collider>();
	character4->GetComponent<Collider>()->SetShape(colliderShape);
	character4->GetComponent<Collider>()->isStatic = true;

	physics->RegisterEntity(character4);
	renderer->RegisterEntity(character4);

	//Object for cube
	std::shared_ptr<Entity> cube = m_EntityManager->CreateEntity<Entity>();
	cube->AddComponent<Transform>();
	cube->GetComponent<Transform>()->SetParent(character->GetComponent<Transform>());
	cube->AddComponent<Material>();
	cube->GetComponent<Material>()->SetShader(shader);

	cube->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1.0f, 20.0f, -20.0f));
	cube->GetComponent<Transform>()->SetLocalRotation(glm::vec3(0.0f, 0.0f, 45.0f));
	cube->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.5f, 1.75f, 1.0f));

	cameraRoot->GetComponent<CameraFollow>()->objectToFollow = character->GetComponent<Transform>();

	physics->RegisterEntity(cube);

	renderer->SetCamera(camera);
	renderer->debugMode = true;
}