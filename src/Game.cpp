#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "TimeCustom.h"
#include "InputHandler.h"
#include "Collider.h"
#include "MapLoader.h"
#include "Scripts/ColorChanger.h"
#include "Scripts/HelloTriggers.h"
#include "Scripts/CameraFollow.h"
#include "Scripts/Player.h"
#include "Scripts/Weapon.h"
#include "Scripts/Character.h"
#include "Scripts/WeaponOnTheGround.h"
#include <time.h>

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

	srand(time(NULL));
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
		TimeCustom::RunTimer();
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

void Game::LoadMap(Renderer* renderer, AssetManager* assetManager, Physics* physics)
{
	std::vector<std::shared_ptr<Entity>> map;
	enum tileEnum
	{
		tree,
		rock,
		grass
	};
	float pos[3];
	float scale[3];
	int x = rand() % 7 + 5;
	int y = rand() % 7 + 5;
	std::string name;
	Model* tileModels[6];
	tileModels[0] = assetManager->LoadModel("./res/models/tiles/Grass/Grass.obj");
	tileModels[1] = assetManager->LoadModel("./res/models/tiles/Tree/Tree.obj");
	tileModels[2] = assetManager->LoadModel("./res/models/tiles/Tree/Birch.obj");
	tileModels[3] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	tileModels[4] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock2.obj");
	tileModels[5] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock3.obj");
	std::cout << "\nX: " << x << " Y: " << y << std::endl;
	x = 2;
	y = 2;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			int random = rand()%2;
			std::ifstream file;
			if (random == 0)
			{
				std::cout << "\nForest 1 loading \n";
				file.open("./res/maps/TileForest.txt");
				if (!file)
				{
					std::cout << "Unable to open file txt";
				}
			}
			else 
			{
				std::cout << "Forest 2 loading \n";
				file.open("./res/maps/TileForest2.txt");
				if (!file)
				{
					std::cout << "Unable to open file txt";
				}
			}
			std::shared_ptr<Entity> tile = m_EntityManager->CreateEntity<Entity>();
			tile->AddComponent<Transform>();

			map.push_back(tile);
			renderer->RegisterEntity(tile);
			physics->RegisterEntity(tile);
			while (file >> name >> pos[0] >> pos[1] >> pos[2] >> scale[0] >> scale[1] >> scale[2])
			{
				std::cout << "\nName: " + name;
				std::cout << "\nPosition: " << pos[0] << " " << pos[1] << " " << pos[2];
				std::cout << "\nScale: " << scale[0] << " " << scale[1] << " " << scale[2];
				std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();
				object->AddComponent<Transform>();
				object->GetComponent<Transform>()->SetParent(tile->GetComponent<Transform>());
				object->GetComponent<Transform>()->SetLocalPosition(glm::vec3(pos[0], pos[1], pos[2]));
				object->GetComponent<Transform>()->SetLocalScale(glm::vec3(scale[0], scale[1], scale[2]));


				object->AddComponent<Mesh>();
				object->GetComponent<Mesh>()->indices = (FindModelByName(tileModels, name))->GetMeshes()[0].indices;
				object->GetComponent<Mesh>()->vertices = (FindModelByName(tileModels, name))->GetMeshes()[0].vertices;
				object->GetComponent<Mesh>()->material = (FindModelByName(tileModels, name))->GetMeshes()[0].material;
				object->GetComponent<Mesh>()->setupMesh();

				map.push_back(object);
				renderer->RegisterEntity(object);
				physics->RegisterEntity(object);
			}
			file.close();
			tile->GetComponent<Transform>()->SetLocalPosition(glm::vec3(i*15,0,j*15));

		}
	}

	
	
	
	//land->GetComponent<Transform>()->SetLocalRotation(glm::vec3(90, 0, 0));
}
Model* Game::FindModelByName(Model* array[], std::string name)
{
	if (name == "Grass")
		return array[0];
	else if (name == "Tree")
		return array[1];
	else if (name == "Birch")
		return array[2];
	else if (name == "Rock1")
		return array[3];
	else if (name == "Rock2")
		return array[4];
	else if (name == "Rock3")
		return array[5];
}

void Game::EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem)
{
	InitializeWeapons(assetManager);
	
	Shader* shader = new Shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
	Shader* shadera = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");

	Model* testModel = assetManager->LoadModel("./res/models/man/model.dae");
	Model* mapModel = assetManager->LoadModel("./res/models/map/Map1.obj");
	Model* testModela = assetManager->LoadModel("./res/models/man/model.dae");
	
	LoadMap(renderer, assetManager, physics);
	//Camera object
	std::shared_ptr<Entity> cameraRoot = m_EntityManager->CreateEntity<Entity>();
	//cameraRoot->GetComponent<Transform>()->SetLocalRotation(glm::vec3(-45.0f, 0, 0));

	cameraRoot->AddComponent<CameraFollow>();
	//cameraRoot->GetComponent<Transform>()->DisplayDebugInfo(true);
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
	//camera->GetComponent<Transform>()->DisplayDebugInfo(true);
	

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
	//character->GetComponent<Transform>()->DisplayDebugInfo(true);

	/*
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
	renderer->RegisterEntity(map4); */

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
	character2->AddComponent<Model>();

	character2->GetComponent<Model>()->UseModel(testModel);
	character2->GetComponent<Mesh>()->SetAll(testModel->GetMeshes()[0]);
	character2->GetComponent<Mesh>()->material->SetShader(shadera);

	character2->GetComponent<Mesh>()->setupMeshfBones();

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

	physics->RegisterEntity(character2);
	renderer->RegisterEntity(character2);

	//nanosuit 3
	std::shared_ptr<Entity> character3 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	character3->AddComponent<Transform>();

	character3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.5f, 0.0f, 0.5f));
	character3->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character3->AddComponent<Mesh>();
	character3->AddComponent<Model>();

	character3->GetComponent<Model>()->UseModel(testModel);
	character3->GetComponent<Mesh>()->SetAll(testModel->GetMeshes()[0]);
	character3->GetComponent<Mesh>()->material->SetShader(shadera);

	character3->GetComponent<Mesh>()->setupMeshfBones();

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
	character3->AddComponent<Character>();

	//gameLogic->RegisterEntity(character3);
	physics->RegisterEntity(character3);
	renderer->RegisterEntity(character3);

	//nanosuit 4
	std::shared_ptr<Entity> character4 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	character4->AddComponent<Transform>();
	character4->GetComponent<Transform>()->SetLocalPosition(glm::vec3(-0.0f, 0.0f, 3.0f));
	character4->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.1f, 0.1f, 0.1f));

	character4->AddComponent<Mesh>();
	character4->AddComponent<Model>();

	character4->GetComponent<Model>()->UseModel(testModel);
	character4->GetComponent<Mesh>()->SetAll(testModel->GetMeshes()[0]);
	character4->GetComponent<Mesh>()->material->SetShader(shadera);

	character4->GetComponent<Mesh>()->setupMeshfBones();

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
	character4->AddComponent<Character>();

	//gameLogic->RegisterEntity(character4);
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

	//testing weapon 1
	std::shared_ptr<Entity> weaponOnTheGround1 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround1->AddComponent<Transform>();
	weaponOnTheGround1->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 3.0f));
	weaponOnTheGround1->AddComponent<Material>();
	weaponOnTheGround1->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround1->AddComponent<WeaponOnTheGround>();

	gameLogic->RegisterEntity(weaponOnTheGround1);
	physics->RegisterEntity(weaponOnTheGround1);
	renderer->RegisterEntity(weaponOnTheGround1);

	//testing weapon 2
	std::shared_ptr<Entity> weaponOnTheGround2 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround2->AddComponent<Transform>();
	weaponOnTheGround2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 5.0f));
	weaponOnTheGround2->AddComponent<Material>();
	weaponOnTheGround2->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround2->AddComponent<WeaponOnTheGround>();

	gameLogic->RegisterEntity(weaponOnTheGround2);
	physics->RegisterEntity(weaponOnTheGround2);
	renderer->RegisterEntity(weaponOnTheGround2);

	//testing weapon 3
	std::shared_ptr<Entity> weaponOnTheGround3 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround3->AddComponent<Transform>();
	weaponOnTheGround3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 7.0f));
	weaponOnTheGround3->AddComponent<Material>();
	weaponOnTheGround3->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround3->AddComponent<WeaponOnTheGround>();

	gameLogic->RegisterEntity(weaponOnTheGround3);
	physics->RegisterEntity(weaponOnTheGround3);
	renderer->RegisterEntity(weaponOnTheGround3);

	//testing weapon 4
	std::shared_ptr<Entity> weaponOnTheGround4 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround4->AddComponent<Transform>();
	weaponOnTheGround4->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 9.0f));
	weaponOnTheGround4->AddComponent<Material>();
	weaponOnTheGround4->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround4->AddComponent<WeaponOnTheGround>();

	gameLogic->RegisterEntity(weaponOnTheGround4);
	physics->RegisterEntity(weaponOnTheGround4);
	renderer->RegisterEntity(weaponOnTheGround4);
}

void Game::InitializeWeapons(AssetManager* assetManager)
{
	Mesh* axeMesh = new Mesh();

	Model* axe = assetManager->LoadModel("./res/models/weapons/axe.obj");

	axeMesh->indices = axe->GetMeshes()[0].indices;
	axeMesh->vertices = axe->GetMeshes()[0].vertices;
	axeMesh->material = axe->GetMeshes()[0].material;
	WeaponFactory::SetWeapon(axeMesh, Axe);

	Mesh* maceMesh = new Mesh();

	Model* mace = assetManager->LoadModel("./res/models/weapons/mace.obj");

	maceMesh->indices = mace->GetMeshes()[0].indices;
	maceMesh->vertices = mace->GetMeshes()[0].vertices;
	maceMesh->material = mace->GetMeshes()[0].material;
	WeaponFactory::SetWeapon(maceMesh, Mace);

	Mesh* swordMesh = new Mesh();

	Model* sword = assetManager->LoadModel("./res/models/weapons/sword.obj");

	swordMesh->indices = sword->GetMeshes()[0].indices;
	swordMesh->vertices = sword->GetMeshes()[0].vertices;
	swordMesh->material = sword->GetMeshes()[0].material;
	WeaponFactory::SetWeapon(swordMesh, Sword);
}