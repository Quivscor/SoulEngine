#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "TimeCustom.h"
#include "InputHandler.h"
#include "Collider.h"
#include "AudioMaster.h"
#include "Listener.h"
#include "Source.h"
#include "MapLoader.h"
#include "Scripts/ColorChanger.h"
#include "Scripts/HelloTriggers.h"
#include "Scripts/CameraFollow.h"
#include "Scripts/Weapon.h"
#include "Scripts/Player.h"
#include "Scripts/Character.h"
#include "Scripts/WeaponOnTheGround.h"
#include "Scripts/Water.h"
#include "Scripts/Village.h"
#include "InstanceManager.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include "GUI/Text.h"
#include "Scripts/WeaponComparator.h"
#include<Billboard.h>
#include "Scripts/PlayerEnemyCommunicator.h"

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
	Shader* shader = new Shader("./res/shaders/basiclight.vert", "./res/shaders/basiclight.frag");
	Shader* screenShader = new Shader("./res/shaders/screen.vert", "./res/shaders/screen.frag");
	Shader* skyBoxShader = new Shader("./res/shaders/skyboxShader.vert", "./res/shaders/skyboxShader.frag");
	Shader* refractorShader = new Shader("./res/shaders/refractorShader.vert", "./res/shaders/refractorShader.frag");
	
	//Creating systems
	AssetManager* assetManager = new AssetManager();
	Model* crystal = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	Renderer* renderer = new Renderer(shader, screenShader, skyBoxShader, refractorShader, crystal);
	Physics* physics = new Physics();
	InputSystem* inputSystem = new InputSystem();
	GameLogic* gameLogic = new GameLogic();
	AudioMaster audioMaster;
	Source source;
	
	Listener listener;
	//Input register test
	std::shared_ptr<Entity> inputHandler = m_EntityManager->CreateEntity<Entity>();
	inputHandler->AddComponent<InputHandler>();
	inputSystem->RegisterEntity(inputHandler);

	EntitiesInit(assetManager, renderer, physics, gameLogic, inputHandler);

	InitializeBasicGUI(renderer, physics);

	gameLogic->Start();
	source.SetLooping(true);
	source.Play(audioMaster.GenBuffer("./res/sound/VikingMusic.wav"));
	source.SetVolume(0.1f);
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

void Game::LoadMap(Renderer* renderer, AssetManager* assetManager, Physics* physics, Shader* animShader, Shader* grassShader, GameLogic* gameLogic, std::shared_ptr<Entity> player)
{
	std::vector<std::shared_ptr<Entity>> map;

	std::vector<glm::vec2> colliderShape;
	colliderShape.push_back({ -2.0f, -1.25f });
	colliderShape.push_back({ -1.25f, -2.0f });
	colliderShape.push_back({ 1.25f, -2.0f });
	colliderShape.push_back({ 2.0f, -1.25f });
	colliderShape.push_back({ 2.0f, 1.25f });
	colliderShape.push_back({ 1.25f,  2.0f });
	colliderShape.push_back({ -1.25f, 2.0f });
	colliderShape.push_back({ -2.0f, 1.25f });

	std::vector<glm::vec2> characterCollider;
	characterCollider.push_back({ -8.0f, -5.0f });
	characterCollider.push_back({ -5.0f, -8.0f });
	characterCollider.push_back({ 5.0f, -8.0f });
	characterCollider.push_back({ 8.0f, -5.0f });
	characterCollider.push_back({ 8.0f, 5.0f });
	characterCollider.push_back({ 5.0f,  8.0f });
	characterCollider.push_back({ -5.0f, 8.0f });
	characterCollider.push_back({ -8.0f, 5.0f });

	
	float pos[3];
	float scale[3];
	int x = rand() % 7 + 5;
	int y = rand() % 7 + 5;
	std::string name;
	Model* tileModels[13];
	tileModels[0] = assetManager->LoadModel("./res/models/tiles/Grass/Grass.obj");
	tileModels[1] = assetManager->LoadModel("./res/models/tiles/Tree/Tree.obj");
	tileModels[2] = assetManager->LoadModel("./res/models/tiles/Tree/Birch.obj");
	tileModels[3] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	tileModels[4] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock2.obj");
	tileModels[5] = assetManager->LoadModel("./res/models/tiles/Rocks/Rock3.obj");
	tileModels[6] = assetManager->LoadModel("./res/models/tiles/Houses/house_1.obj");
	tileModels[7] = assetManager->LoadModel("./res/models/tiles/Houses/house_2.obj");
	tileModels[8] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	tileModels[9] = assetManager->LoadModel("./res/models/player/player_idle.dae");
	tileModels[10] = assetManager->LoadModel("./res/models/player/player_run.dae");
	tileModels[11] = assetManager->LoadModel("./res/models/player/player_attack.dae");
	tileModels[12] = assetManager->LoadModel("./res/models/player/player_death.dae");

	Shader* shaderForEnemy = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");

	Model* grassLeaf = assetManager->LoadModel("./res/models/tiles/Grass/GrassFromGalapagos.obj");
	InstanceManager* grassM = new InstanceManager(grassLeaf);
	std::shared_ptr <InstanceManager> grassManager(grassM);
	name = "GrassLeaf";
	int numberOfGrass = grassManager->amount;
	srand(time(NULL));
	//grassManager->instanceModels = new glm::mat4[numberOfGrass];
	for (int i = 0; i < numberOfGrass; i++)
	{
		float randomX = rand() % 40 + (rand()% 100)/100.f;
		if (rand() % 2 == 0)
			randomX *= -1;
		float randomY = rand() % 40 + (rand() % 100)/100.f;
		if (rand() % 2 == 0)
			randomY *= -1;
		glm::vec3 pos(randomX, 0, randomY);
		glm::vec3 scale(0.05, 0.3, 0.05);
		glm::vec3 rot(0, 0, 0);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pos));
		model = glm::scale(model, glm::vec3(scale));
		//grassManager->AddParameters(pos, scale, rot, model);
		std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();
		grassManager->instanceModels[i] = model;

		object->AddComponent<Mesh>();
		object->GetComponent<Mesh>()->indices = (grassLeaf)->GetMeshes()[0].indices;
		object->GetComponent<Mesh>()->vertices = (grassLeaf)->GetMeshes()[0].vertices;
		object->GetComponent<Mesh>()->material = (grassLeaf)->GetMeshes()[0].material;
		object->GetComponent<Mesh>()->setupMesh();
		grassManager->m_mesh = object->GetComponent<Mesh>().get();
		//object->GetComponent<Mesh>()->material->SetShader(grassShader);
		
	}
	std::cout << sizeof(grassManager->instanceModels);
	grassManager->m_shader = grassShader;
	glGenBuffers(1, &grassManager->buffer);
	glBindBuffer(GL_ARRAY_BUFFER, grassManager->buffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfGrass * sizeof(glm::mat4), &grassManager->instanceModels[0], GL_STATIC_DRAW);
	unsigned int VAO = grassLeaf->meshes[0].GetVAO();
	glBindVertexArray(VAO);
	// set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	renderer->RegisterManager(grassManager);
	//std::cout << "\nX: " << x << " Y: " << y << std::endl;
	x = 5;
	y = 5;
	int random = 0;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (i == 0 && j == 0)
				random = 0;
			else
				random = rand()%4;
			std::ifstream file;
			if (random == 0)
			{
				std::cout << "\nForest 1 loading \n";
				file.open("./res/maps/TileForest.txt");
				if (!file)
				{
					std::cout << "Unable to open TileForest.txt";
				}
			}
			if(random == 1)
			{
				std::cout << "Forest 2 loading \n";
				file.open("./res/maps/TileForest2.txt");
				if (!file)
				{
					std::cout << "Unable to open file Forest2.txt";
				}
			}
			if (random == 2)
			{
				std::cout << "Village loading \n";
				file.open("./res/maps/TileVillage.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileVillage.txt";
				}
			}
			if (random == 3)
			{
				std::cout << "Mountain loading \n";
				file.open("./res/maps/TileMountain.txt");
				if (!file)
				{
					std::cout << "Unable to open file TileMountain.txt";
				}
			}
			//random++;

			std::shared_ptr<Entity> tile = m_EntityManager->CreateEntity<Entity>();
			tile->AddComponent<Transform>();

			map.push_back(tile);
			renderer->RegisterEntity(tile);
			physics->RegisterEntity(tile);

			if (random == 2)
			{
				//std::shared_ptr<Entity> object = m_EntityManager->CreateEntity<Entity>();

				//object->AddComponent<Transform>();
				//object->GetComponent<Transform>()->SetParent(tile->GetComponent<Transform>());
				//object->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
				//object->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));

				tile->AddComponent<Village>();
				//physics->RegisterEntity(object);
				gameLogic->RegisterEntity(tile);
			}

			while (file >> name >> pos[0] >> pos[1] >> pos[2] >> scale[0] >> scale[1] >> scale[2])
			{
				//std::cout << "\nName: " + name;
				//std::cout << "\nPosition: " << pos[0] << " " << pos[1] << " " << pos[2];
				//std::cout << "\nScale: " << scale[0] << " " << scale[1] << " " << scale[2];
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

				if (name == "Grass")
				{
					object->layer = Layer::GroundLayer;
				}
				if (name == "Tree" || name == "Birch")
				{
					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(colliderShape);
					object->GetComponent<Collider>()->isStatic = true;
				}
				if (name == "Enemy")
				{
					object->AddComponent<Model>();

					object->GetComponent<Model>()->UseModel((FindModelByName(tileModels, name)));
					object->GetComponent<Mesh>()->SetAll((FindModelByName(tileModels, name))->GetMeshes()[0]);
					object->GetComponent<Mesh>()->material->SetShader(animShader);
					object->GetComponent<Mesh>()->setupMeshfBones();

					object->AddComponent<Collider>();
					object->GetComponent<Collider>()->SetShape(characterCollider);
					object->AddComponent<Character>();
					object->GetComponent<Character>()->playerReference = player;

					object->layer = EnemyLayer;
					
					//container for Enemy script
					std::shared_ptr<Entity> enemy = m_EntityManager->CreateEntity<Entity>();
					enemy->AddComponent<Transform>();
					enemy->GetComponent<Transform>()->SetParent(object->GetComponent<Transform>());
					enemy->AddComponent<Enemy>();
					enemy->GetComponent<Enemy>()->animationIdle = FindModelByName(tileModels, "EnemyIdle");
					enemy->GetComponent<Enemy>()->animationRun = FindModelByName(tileModels, "EnemyRun");
					enemy->GetComponent<Enemy>()->animationAttack = FindModelByName(tileModels, "EnemyAttack");
					enemy->GetComponent<Enemy>()->animationDeath = FindModelByName(tileModels, "EnemyDeath");
					enemy->GetComponent<Enemy>()->shader = shaderForEnemy;

					gameLogic->RegisterEntity(enemy);
					physics->RegisterEntity(enemy);

					if (random == 2)
					{
						tile->GetComponent<Village>()->enemiesInVillage.push_back(enemy->GetComponent<Enemy>());
					}
				}

				map.push_back(object);
				physics->RegisterEntity(object);
				renderer->RegisterEntity(object);
				
			}

			file.close();
			
			tile->GetComponent<Transform>()->SetLocalPosition(glm::vec3(i*16,0,j*16));

		}
	}
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
	else if (name == "House1")
		return array[6];
	else if (name == "House2")
		return array[7];
	else if (name == "Enemy")
		return array[8];
	else if (name == "EnemyIdle")
		return array[9];
	else if (name == "EnemyRun")
		return array[10];
	else if (name == "EnemyAttack")
		return array[11];
	else if (name == "EnemyDeath")
		return array[12];
	else if (name == "GrassLeaf")
		return array[13];
}

void Game::EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem)
{
	InitializeWeapons(assetManager);
	
	Shader* shader = new Shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
	Shader* shadera = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");
	Shader* grassShader = new Shader("./res/shaders/grass.vert", "./res/shaders/grass.frag");
	//Shader* light = new Shader("./res/shaders/bassiclight.vert", "./res/shaders/basiclight.frag");

	//Model* mapModel = assetManager->LoadModel("./res/models/map/Map1.obj");
	
	//player
	//sword
	Model* playerSwordIdle = assetManager->LoadModel("./res/models/player/sword/player_idle_sword.dae");
	Model* playerSwordAttack = assetManager->LoadModel("./res/models/player/sword/player_attack_sword.dae");
	Model* playerSwordRun = assetManager->LoadModel("./res/models/player/sword/player_run_sword.dae");
	Model* playerSwordRoll = assetManager->LoadModel("./res/models/player/sword/player_roll_sword.dae");
	//axe
	Model* playerAxeIdle = assetManager->LoadModel("./res/models/player/axe/player_idle_axe.dae");
	Model* playerAxeAttack = assetManager->LoadModel("./res/models/player/axe/player_attack_axe.dae");
	Model* playerAxeRun = assetManager->LoadModel("./res/models/player/axe/player_run_axe.dae");
	Model* playerAxeRoll = assetManager->LoadModel("./res/models/player/axe/player_roll_axe.dae");
	//mace
	Model* playerMaceIdle = assetManager->LoadModel("./res/models/player/mace/player_idle_mace.dae");
	Model* playerMaceAttack = assetManager->LoadModel("./res/models/player/mace/player_attack_mace.dae");
	Model* playerMaceRun = assetManager->LoadModel("./res/models/player/mace/player_run_mace.dae");
	Model* playerMaceRoll = assetManager->LoadModel("./res/models/player/mace/player_roll_mace.dae");

	Model* playerDeath = assetManager->LoadModel("./res/models/player/player_death.dae");

	//Camera object
	std::shared_ptr<Entity> cameraRoot = m_EntityManager->CreateEntity<Entity>();
	cameraRoot->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 10, 0));
	//cameraRoot->GetComponent<Transform>()->SetLocalRotation(glm::vec3(-45.0f, 0, 0));

	cameraRoot->AddComponent<CameraFollow>();
	//cameraRoot->GetComponent<Transform>()->DisplayDebugInfo(true);
	physics->RegisterEntity(cameraRoot);
	gameLogic->RegisterEntity(cameraRoot);

	std::shared_ptr<Entity> camera = m_EntityManager->CreateEntity<Entity>();
	camera->AddComponent<Camera>();
	physics->RegisterEntity(camera);
	renderer->SetCamera(camera);
	renderer->debugMode = false;

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
	character->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	character->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.08f, 0.08f, 0.08f));
	//character->GetComponent<Transform>()->DisplayDebugInfo(true);

	//player
	character->AddComponent<Mesh>();
	character->AddComponent<Model>();

	character->GetComponent<Model>()->UseModel(playerSwordIdle);
	character->GetComponent<Mesh>()->SetAll(playerSwordIdle->GetMeshes()[0]);
	character->GetComponent<Mesh>()->setupMeshfBones();

	character->GetComponent<Mesh>()->material->SetShader(shadera);

	character->AddComponent<Player>();
	character->GetComponent<Player>()->inputHandler = inputSystem;
	character->GetComponent<Player>()->animationSwordRun = playerSwordRun;
	character->GetComponent<Player>()->animationSwordAttack = playerSwordAttack;
	character->GetComponent<Player>()->animationSwordIdle = playerSwordIdle;
	character->GetComponent<Player>()->animationSwordRoll = playerSwordRoll;
	character->GetComponent<Player>()->animationAxeRun = playerAxeRun;
	character->GetComponent<Player>()->animationAxeAttack = playerAxeAttack;
	character->GetComponent<Player>()->animationAxeIdle = playerAxeIdle;
	character->GetComponent<Player>()->animationAxeRoll = playerAxeRoll;
	character->GetComponent<Player>()->animationMaceRun = playerMaceRun;
	character->GetComponent<Player>()->animationMaceAttack = playerMaceAttack;
	character->GetComponent<Player>()->animationMaceIdle = playerMaceIdle;
	character->GetComponent<Player>()->animationMaceRoll = playerMaceRoll;
	character->GetComponent<Player>()->animationDeath = playerDeath;
	character->GetComponent<Player>()->shader = shadera;
	character->GetComponent<Player>()->renderer = renderer;
	character->layer = PlayerLayer;


	gameLogic->RegisterEntity(character);

	character->AddComponent<Collider>();
	character->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//Weapons gui 
	InitializePlayerGUI(renderer, physics, character, gameLogic);

	std::shared_ptr<Entity> pec = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	pec->AddComponent<PlayerEnemyCommunicator>();

	character->GetComponent<Player>()->pec = pec->GetComponent<PlayerEnemyCommunicator>();

	LoadMap(renderer, assetManager, physics, shadera, grassShader, gameLogic, pec);
	//player's weapon ! -> it should be spawned in player's script imo
	std::shared_ptr<Entity> weapon = m_EntityManager->CreateEntity<Entity>();
	weapon->AddComponent<Transform>();
	weapon->GetComponent<Transform>()->SetParent(character->GetComponent<Transform>());
	weapon->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 6.0f));

	colliderShape.clear();
	colliderShape.push_back({ -6.0f, -5.0f });
	colliderShape.push_back({ -6.0f, 18.0f });
	colliderShape.push_back({ 6.0f, 18.0f });
	colliderShape.push_back({ 6.0f, -5.0f });

	weapon->AddComponent<Collider>();
	weapon->GetComponent<Collider>()->SetShape(colliderShape);
	weapon->GetComponent<Collider>()->isTrigger = true;
	weapon->AddComponent<Weapon>();

	renderer->RegisterEntity(weapon);
	gameLogic->RegisterEntity(weapon);
	physics->RegisterEntity(weapon);


	//renderer->RegisterEntity(weapon);

	character->GetComponent<Player>()->weapon = weapon->GetComponent<Weapon>();

	//nanosuit 2 
	/*
	std::shared_ptr<Entity> character2 = m_EntityManager->CreateEntity<Entity>();
	character2->AddComponent<Transform>();

	character2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1.0f, 0.0f, 3.0f));
	character2->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.08f, 0.08f, 0.08f));

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
	*/
	//nanosuit 3
	/*
	std::shared_ptr<Entity> character3 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	character3->AddComponent<Transform>();

	character3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.5f, 0.0f, 0.5f));
	character3->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.08f, 0.08f, 0.08f));

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
	character4->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.08f, 0.08f, 0.08f));

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

	physics->RegisterEntity(character4);
	renderer->RegisterEntity(character4);
	*/
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
	//renderer->debugMode = true;

	//testing weapon 1
	std::shared_ptr<Entity> weaponOnTheGround1 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround1->AddComponent<Transform>();
	weaponOnTheGround1->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 3.0f));
	weaponOnTheGround1->AddComponent<Material>();
	weaponOnTheGround1->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround1->AddComponent<WeaponOnTheGround>();
	weaponOnTheGround1->GetComponent<WeaponOnTheGround>()->Start();

	//gameLogic->RegisterEntity(weaponOnTheGround1);
	physics->RegisterEntity(weaponOnTheGround1);
	renderer->RegisterEntity(weaponOnTheGround1);

	//testing weapon 2
	std::shared_ptr<Entity> weaponOnTheGround2 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround2->AddComponent<Transform>();
	weaponOnTheGround2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 5.0f));
	weaponOnTheGround2->AddComponent<Material>();
	weaponOnTheGround2->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround2->AddComponent<WeaponOnTheGround>();
	weaponOnTheGround2->GetComponent<WeaponOnTheGround>()->Start();
	//gameLogic->RegisterEntity(weaponOnTheGround2);
	physics->RegisterEntity(weaponOnTheGround2);
	renderer->RegisterEntity(weaponOnTheGround2);

	//testing weapon 3
	std::shared_ptr<Entity> weaponOnTheGround3 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround3->AddComponent<Transform>();
	weaponOnTheGround3->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 7.0f));
	weaponOnTheGround3->AddComponent<Material>();
	weaponOnTheGround3->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround3->AddComponent<WeaponOnTheGround>();
	weaponOnTheGround3->GetComponent<WeaponOnTheGround>()->Start();
	//gameLogic->RegisterEntity(weaponOnTheGround3);
	physics->RegisterEntity(weaponOnTheGround3);
	renderer->RegisterEntity(weaponOnTheGround3);

	//testing weapon 4
	std::shared_ptr<Entity> weaponOnTheGround4 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponOnTheGround4->AddComponent<Transform>();
	weaponOnTheGround4->GetComponent<Transform>()->SetLocalPosition(glm::vec3(2.0f, 0.0f, 9.0f));
	weaponOnTheGround4->AddComponent<Material>();
	weaponOnTheGround4->GetComponent<Material>()->SetShader(shader);
	weaponOnTheGround4->AddComponent<WeaponOnTheGround>();
	weaponOnTheGround4->GetComponent<WeaponOnTheGround>()->Start();
	//gameLogic->RegisterEntity(weaponOnTheGround4);
	physics->RegisterEntity(weaponOnTheGround4);
	renderer->RegisterEntity(weaponOnTheGround4);
	camera->GetComponent<Camera>()->SetLookAndUpVectors(character);

	//water
	std::shared_ptr<Entity> water = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	water->layer = Layer::WaterLayer;
	water->AddComponent<Transform>();
	water->GetComponent<Transform>()->SetLocalPosition(glm::vec3(-20.0f, -2.0f, -25.0f));
	water->AddComponent<Water>();
	water->GetComponent<Water>()->CreateShape();

	physics->RegisterEntity(water);
	renderer->RegisterEntity(water);
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

void Game::InitializeBasicGUI(Renderer* renderer, Physics* physics)
{
	std::shared_ptr<Entity> movement = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	movement->AddComponent<Transform>();
	movement->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 680, 0));
	movement->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	movement->AddComponent<Text>();
	movement->GetComponent<Text>()->text = "WSAD - movement";
	movement->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(movement);
	renderer->RegisterEntity(movement);

	std::shared_ptr<Entity> interaction = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	interaction->AddComponent<Transform>();
	interaction->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 660, 0));
	interaction->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	interaction->AddComponent<Text>();
	interaction->GetComponent<Text>()->text = "E - use";
	interaction->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(interaction);
	renderer->RegisterEntity(interaction);

	std::shared_ptr<Entity> attack = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	attack->AddComponent<Transform>();
	attack->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 640, 0));
	attack->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	attack->AddComponent<Text>();
	attack->GetComponent<Text>()->text = "J - attack";
	attack->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(attack);
	renderer->RegisterEntity(attack);

	std::shared_ptr<Entity> roll = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	roll->AddComponent<Transform>();
	roll->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0, 620, 0));
	roll->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	roll->AddComponent<Text>();
	roll->GetComponent<Text>()->text = "K - roll";
	roll->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(roll);
	renderer->RegisterEntity(roll);
}

void Game::InitializePlayerGUI(Renderer* renderer, Physics* physics, std::shared_ptr<Entity> player, GameLogic* gameLogic)
{
	std::shared_ptr<Entity> damageDesc = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	damageDesc->AddComponent<Transform>();
	damageDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(930, 60, 0));
	damageDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	damageDesc->AddComponent<Text>();
	damageDesc->GetComponent<Text>()->text = "Damage (+%)";
	damageDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(damageDesc);
	renderer->RegisterEntity(damageDesc);

	std::shared_ptr<Entity> oldDamage = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldDamage->AddComponent<Transform>();
	oldDamage->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 60, 0));
	oldDamage->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	oldDamage->AddComponent<Text>();
	oldDamage->GetComponent<Text>()->text = "XX";
	oldDamage->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldDamage);
	renderer->RegisterEntity(oldDamage);

	std::shared_ptr<Entity> damageCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	damageCompare->AddComponent<Transform>();
	damageCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 60, 0));
	damageCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	damageCompare->AddComponent<Text>();
	damageCompare->GetComponent<Text>()->text = "->";
	damageCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(damageCompare);
	renderer->RegisterEntity(damageCompare);

	std::shared_ptr<Entity> newDamage = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newDamage->AddComponent<Transform>();
	newDamage->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 60, 0));
	newDamage->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	newDamage->AddComponent<Text>();
	newDamage->GetComponent<Text>()->text = "XX";
	newDamage->GetComponent<Text>()->color = glm::vec3(0.0f, 1.0f, 0.0f);

	physics->RegisterEntity(newDamage);
	renderer->RegisterEntity(newDamage);

	// ATTACK SPEED

	std::shared_ptr<Entity> speedDesc = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	speedDesc->AddComponent<Transform>();
	speedDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(930, 40, 0));
	speedDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	speedDesc->AddComponent<Text>();
	speedDesc->GetComponent<Text>()->text = "Att. Speed (+%)";
	speedDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(speedDesc);
	renderer->RegisterEntity(speedDesc);

	std::shared_ptr<Entity> oldSpeed = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldSpeed->AddComponent<Transform>();
	oldSpeed->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 40, 0));
	oldSpeed->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	oldSpeed->AddComponent<Text>();
	oldSpeed->GetComponent<Text>()->text = "XX";
	oldSpeed->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldSpeed);
	renderer->RegisterEntity(oldSpeed);

	std::shared_ptr<Entity> speedCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	speedCompare->AddComponent<Transform>();
	speedCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 40, 0));
	speedCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	speedCompare->AddComponent<Text>();
	speedCompare->GetComponent<Text>()->text = "->";
	speedCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(speedCompare);
	renderer->RegisterEntity(speedCompare);

	std::shared_ptr<Entity> newSpeed = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newSpeed->AddComponent<Transform>();
	newSpeed->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 40, 0));
	newSpeed->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	newSpeed->AddComponent<Text>();
	newSpeed->GetComponent<Text>()->text = "XX";
	newSpeed->GetComponent<Text>()->color = glm::vec3(0.0f, 1.0f, 0.0f);

	physics->RegisterEntity(newSpeed);
	renderer->RegisterEntity(newSpeed);

	// DURABILITY

	std::shared_ptr<Entity> durDesc = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	durDesc->AddComponent<Transform>();
	durDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(930, 20, 0));
	durDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	durDesc->AddComponent<Text>();
	durDesc->GetComponent<Text>()->text = "Durability";
	durDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(durDesc);
	renderer->RegisterEntity(durDesc);

	std::shared_ptr<Entity> oldDur = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldDur->AddComponent<Transform>();
	oldDur->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 20, 0));
	oldDur->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	oldDur->AddComponent<Text>();
	oldDur->GetComponent<Text>()->text = "XX";
	oldDur->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldDur);
	renderer->RegisterEntity(oldDur);

	std::shared_ptr<Entity> durCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	durCompare->AddComponent<Transform>();
	durCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 20, 0));
	durCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	durCompare->AddComponent<Text>();
	durCompare->GetComponent<Text>()->text = "->";
	durCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(durCompare);
	renderer->RegisterEntity(durCompare);

	std::shared_ptr<Entity> newDur = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newDur->AddComponent<Transform>();
	newDur->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 20, 0));
	newDur->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.35f, 0.35f, 0.35f));
	newDur->AddComponent<Text>();
	newDur->GetComponent<Text>()->text = "XX";
	newDur->GetComponent<Text>()->color = glm::vec3(0.0f, 1.0f, 0.0f);

	physics->RegisterEntity(newDur);
	renderer->RegisterEntity(newDur);


	//WEAPON COMPARATOR

	std::shared_ptr<Entity> weaponComparator = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	weaponComparator->AddComponent<WeaponComparator>();

	weaponComparator->GetComponent<WeaponComparator>()->damageTitle = damageDesc;
	weaponComparator->GetComponent<WeaponComparator>()->oldDamage = oldDamage;
	weaponComparator->GetComponent<WeaponComparator>()->damageCompare = damageCompare;
	weaponComparator->GetComponent<WeaponComparator>()->newDamage = newDamage;

	weaponComparator->GetComponent<WeaponComparator>()->speedTitle = speedDesc;
	weaponComparator->GetComponent<WeaponComparator>()->oldASpeed = oldSpeed;
	weaponComparator->GetComponent<WeaponComparator>()->speedCompare = speedCompare;
	weaponComparator->GetComponent<WeaponComparator>()->newASpeed = newSpeed;

	weaponComparator->GetComponent<WeaponComparator>()->durabilityTitle = durDesc;
	weaponComparator->GetComponent<WeaponComparator>()->oldDurability = oldDur;
	weaponComparator->GetComponent<WeaponComparator>()->durabilityCompare = durCompare;
	weaponComparator->GetComponent<WeaponComparator>()->newDurability = newDur;

	weaponComparator->GetComponent<WeaponComparator>()->Show(false);

	player->GetComponent<Player>()->weaponComparator = weaponComparator->GetComponent<WeaponComparator>();



	// others

	std::shared_ptr<Entity> berserkerMode = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	berserkerMode->AddComponent<Transform>();
	berserkerMode->GetComponent<Transform>()->SetLocalPosition(glm::vec3(550, 650, 0));
	berserkerMode->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	berserkerMode->AddComponent<Text>();
	berserkerMode->GetComponent<Text>()->text = "BERSERKER!";
	berserkerMode->GetComponent<Text>()->color = glm::vec3(0.6f, 0.2f, 1.0f);

	physics->RegisterEntity(berserkerMode);
	renderer->RegisterEntity(berserkerMode);

	berserkerMode->isActive = false;

	player->GetComponent<Player>()->berserkerModeText = berserkerMode;
}