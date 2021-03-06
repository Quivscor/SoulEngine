﻿#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"
#include "TimeCustom.h"
#include "InputHandler.h"
#include "Collider.h"

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
#include "Scripts/KratosAxe.h"
#include "Scripts/PlayerEnemyCommunicator.h"
#include "Scripts/GameManager.h"


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
	m_Window->Init("SoulEater", 1920, 1080);

	//LogEvent e("Hello events!");
	//m_Window->EventCallback(e);

	srand(time(NULL));
}

void Game::Run()
{	
	//-----------------------------Tests zone------------------------------------------
	//TO DELETE:
	//Creating simple shader
	std::cout << "=== SoulEngine starting ===\n";

	std::cout << "\n=== Creating shaders\n";
	Shader* shader = new Shader("./res/shaders/basiclight.vert", "./res/shaders/basiclight.frag");
	std::cout << "- basiclight shader created\n";
	Shader* screenShader = new Shader("./res/shaders/screen.vert", "./res/shaders/screen.frag");
	std::cout << "- screen shader created\n";
	Shader* skyBoxShader = new Shader("./res/shaders/skyboxShader.vert", "./res/shaders/skyboxShader.frag");
	std::cout << "- skybox shader created\n";
	Shader* refractorShader = new Shader("./res/shaders/refractorShader.vert", "./res/shaders/refractorShader.frag");
	std::cout << "- refractor shader created\n";

	//Creating systems
	std::cout << "\n=== Creating systems\n";
	AssetManager* assetManager = new AssetManager();
	std::cout << "- AssetManager created\n";
	Model* crystal = assetManager->LoadModel("./res/models/tiles/Rocks/Rock1.obj");
	Renderer* renderer = new Renderer(shader, screenShader, skyBoxShader, refractorShader, crystal);
	std::cout << "- Renderer created\n";
	Physics* physics = new Physics();
	std::cout << "- Physics created\n";
	InputSystem* inputSystem = new InputSystem();
	std::cout << "- InputSystem created\n";
	GameLogic* gameLogic = new GameLogic();
	std::cout << "- GameLogic created\n";
	MapGenerator* mapGenerator = NULL;
	std::cout << "- MapGenerator created\n";
	AudioMaster *audioMaster = new AudioMaster();
	std::cout << "- AudioMaster created\n";
	Source source;

	Listener listener;
	//Input register test
	std::cout << "\n=== Initalizing\n";
	std::shared_ptr<Entity> inputHandler = m_EntityManager->CreateEntity<Entity>();

	inputHandler->AddComponent<InputHandler>();

	inputSystem->RegisterEntity(inputHandler);
	std::cout << "- inputSystem registered\n";
	EntitiesInit(assetManager, renderer, physics, gameLogic, inputHandler, mapGenerator, audioMaster);

	InitializeBasicGUI(renderer, physics);

	renderer->Init();
	physics->Init();

	std::cout << "\n=== BasicGUI initalized\n";
	gameLogic->Start();
	source.SetLooping(true);
	source.Play(audioMaster->GenBuffer("./res/sound/VikingMusic.wav"));
	source.SetVolume(0.1f);
	//---------------------------------------------------------------------------------

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetMWindow(), false);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	std::chrono::time_point<std::chrono::steady_clock> startFull = std::chrono::steady_clock::now(), 
		endFull = std::chrono::steady_clock::now(), startGL = std::chrono::steady_clock::now(), 
		endGL = std::chrono::steady_clock::now(), startPhy = std::chrono::steady_clock::now(), 
		endPhy = std::chrono::steady_clock::now(), startRender = std::chrono::steady_clock::now(), 
		endRender = std::chrono::steady_clock::now();
	std::chrono::duration<double> timeGameLogic=startGL - endGL, timePhysics=startGL - endGL, timeRender=startGL - endGL, timeFull=startGL - endGL;
	std::cout << "\n=== ImGui initialized\n";


	while (true)
	{
		startFull = std::chrono::steady_clock::now();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//TimeCustom::RunTimer();
		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_SPACE)&& timing ==false)
		{

			
			timing = true;

		}
		if (timing==true)
		{
			TimeCustom::RunTimer();
		}
		//physics->FixedUpdate();
	
		//double start = glfwGetTime();
		glfwPollEvents();
		//input must be early to read from it
		inputSystem->Update();
		
		if(debug)
		{
			ImGui::Begin("Debug window");
			ImGui::Text("%f FPS", 1 / TimeCustom::GetDeltaTime());
			ImGui::Text("%f %% of time on GameLogic Update", timeGameLogic.count()/timeFull.count());
			ImGui::Text("%f %% of time on Physics Update", timePhysics.count()/timeFull.count());
			ImGui::Text("%f %% of time on Graphics Update", timeRender.count()/timeFull.count());
			ImGui::End();
		}

		startGL = std::chrono::steady_clock::now();
		gameLogic->Update();
		endGL = std::chrono::steady_clock::now();
		timeGameLogic = endGL - startGL;

		if (inputHandler->GetComponent<InputHandler>()->GetKeyRepeat(GLFW_KEY_ESCAPE)) {
			glfwTerminate();
			glfwSetWindowShouldClose(m_Window->GetMWindow(), true);
			exit(3);
		}

		startPhy = std::chrono::steady_clock::now();
		physics->Update();
		endPhy = std::chrono::steady_clock::now();
		timePhysics = endPhy - startPhy;
		
		startRender = std::chrono::steady_clock::now();
		renderer->Update();
		endRender = std::chrono::steady_clock::now();
		timeRender = endRender - startRender;

		physics->LateUpdate();
		renderer->LateUpdate();
		inputSystem->LateUpdate();

		endFull = std::chrono::steady_clock::now();
		timeFull = endFull - startFull;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Game::EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem, MapGenerator* mapGenerator, AudioMaster* audioMaster)
{
	InitializeWeapons(assetManager);
	std::cout << "- Weapons initalized\n";
	Shader* shader = new Shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
	std::cout << "- basic shader created\n";
	Shader* shadera = new Shader("./res/shaders/anim.vert", "./res/shaders/anim.frag");
	std::cout << "- anim shader created\n";
	Shader* grassShader = new Shader("./res/shaders/grass.vert", "./res/shaders/grass.frag");
	std::cout << "- grass shader created\n";

	//player
	std::cout << "\n=== Loading sword's animations\n";
	//sword
	Model* playerSwordIdle = assetManager->LoadModel("./res/models/player/sword/player_idle_sword.dae");
	std::cout << "- player_idle animation loaded\n";
	Model* playerSwordAttack = assetManager->LoadModel("./res/models/player/sword/player_attack_sword.dae");
	std::cout << "- player_attack animation loaded\n";
	Model* playerSwordRun = assetManager->LoadModel("./res/models/player/sword/player_run_sword.dae");
	std::cout << "- player_run animation loaded\n";
	Model* playerSwordRoll = assetManager->LoadModel("./res/models/player/sword/player_roll_sword.dae");
	std::cout << "- player_roll animation loaded\n";

	std::cout << "\n=== Loading axe's animations\n";
	//axe
	Model* playerAxeIdle = assetManager->LoadModel("./res/models/player/axe/player_idle_axe.dae");
	std::cout << "- player_idle animation loaded\n";
	Model* playerAxeAttack = assetManager->LoadModel("./res/models/player/axe/player_attack_axe.dae");
	std::cout << "- player_attack animation loaded\n";
	Model* playerAxeRun = assetManager->LoadModel("./res/models/player/axe/player_run_axe.dae");
	std::cout << "- player_run animation loaded\n";
	Model* playerAxeRoll = assetManager->LoadModel("./res/models/player/axe/player_roll_axe.dae");
	std::cout << "- player_roll animation loaded\n";

	std::cout << "\n=== Loading mace's animations\n";
	//mace
	Model* playerMaceIdle = assetManager->LoadModel("./res/models/player/mace/player_idle_mace.dae");
	std::cout << "- player_idle animation loaded\n";
	Model* playerMaceAttack = assetManager->LoadModel("./res/models/player/mace/player_attack_mace.dae");
	std::cout << "- player_attack animation loaded\n";
	Model* playerMaceRun = assetManager->LoadModel("./res/models/player/mace/player_run_mace.dae");
	std::cout << "- player_run animation loaded\n";
	Model* playerMaceRoll = assetManager->LoadModel("./res/models/player/mace/player_roll_mace.dae");
	std::cout << "- player_roll animation loaded\n";

	std::cout << "\n=== Loading without weapon animations\n";
	//without weapon
	Model* playerIdle = assetManager->LoadModel("./res/models/player/noWeapon/player_idle.dae");
	std::cout << "- player_idle animation loaded\n";
	Model* playerAttack = assetManager->LoadModel("./res/models/player/noWeapon/player_attack.dae");
	std::cout << "- player_attack animation loaded\n";
	Model* playerRun = assetManager->LoadModel("./res/models/player/noWeapon/player_run.dae");
	std::cout << "- player_run animation loaded\n";
	Model* playerRoll = assetManager->LoadModel("./res/models/player/noWeapon/player_roll.dae");
	std::cout << "- player_roll animation loaded\n";
	Model* playerDeath = assetManager->LoadModel("./res/models/player/player_death.dae");
	std::cout << "- player_death animation loaded\n";

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
	std::cout << "\n=== Camera initalized \n";

	//LoadMap(5,5, renderer, assetManager, physics);
	/*std::cout<<testModela->scene->mAnimations[0]->mDuration;*/
	//float fTheta = glm::pi<float>() * 2.0f / 5.0f;
	std::vector<glm::vec2> colliderShape { { -5.0f, -5.0f } ,{ -5.0f, 5.0f } ,{ 5.0f, 5.0f },{ 5.0f, -5.0f } };
	/*for (int i = 0; i < 5; i++)
	{
		colliderShape.push_back({ 30.0f * glm::cos(fTheta * i), 30.0f * glm::sin(fTheta * i) });
	}*/

	std::shared_ptr<Entity> berskerMode = m_EntityManager->CreateEntity<Entity>();
	berskerMode->AddComponent<HUD>();
	berskerMode->GetComponent<HUD>()->setFill(false);
	berskerMode->GetComponent<HUD>()->active = false;
	berskerMode->GetComponent<HUD>()->SetHud(2.0f, 2.0f, -1.f, -1.0f, "./res/textures/BerserkerMode2.png");
	renderer->RegisterHUD(berskerMode);


	//Object with model
	std::shared_ptr<Entity> character = m_EntityManager->CreateEntity<Entity>();
	character->AddComponent<Transform>();
	character->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	character->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.08f, 0.08f, 0.08f));
	//character->GetComponent<Transform>()->DisplayDebugInfo(true);

	//player
	character->AddComponent<Mesh>();
	character->AddComponent<Model>();

	character->GetComponent<Model>()->UseModel(playerIdle);
	character->GetComponent<Mesh>()->SetAll(playerIdle->GetMeshes()[0]);
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
	character->GetComponent<Player>()->animationRun = playerRun;
	character->GetComponent<Player>()->animationAttack = playerAttack;
	character->GetComponent<Player>()->animationIdle = playerIdle;
	character->GetComponent<Player>()->animationRoll = playerRoll;
	character->GetComponent<Player>()->animationDeath = playerDeath;
	character->GetComponent<Player>()->shader = shadera;
	character->GetComponent<Player>()->renderer = renderer;
	character->GetComponent<Player>()->audioMaster = audioMaster;
	character->layer = PlayerLayer;
	
	std::shared_ptr<Entity> durabilityMeter = m_EntityManager->CreateEntity<Entity>();
	durabilityMeter->AddComponent<HUD>();
	durabilityMeter->GetComponent<HUD>()->setFill(false);
	durabilityMeter->GetComponent<HUD>()->setTextureColor(true);
	durabilityMeter->GetComponent<HUD>()->setColor(glm::vec4(0.0f, 0, 0, 0.0f));
	durabilityMeter->GetComponent<HUD>()->SetHud(0.08f, 0.4f, 0.8f, 0.0f, "./res/swordfill.png");
	character->GetComponent<Player>()->HUDDurability = durabilityMeter->GetComponent<HUD>();
	character->GetComponent<Player>()->HUDBerserker = berskerMode->GetComponent<HUD>();
	std::shared_ptr<Entity> Start = m_EntityManager->CreateEntity<Entity>();
	renderer->RegisterHUD(durabilityMeter);



	std::cout << "\n=== Animations added to player \n";

	gameLogic->RegisterEntity(character);

	//character->GetComponent<Billboard>()->SetBillboard("./res/textures/ExampleBillboard.DDS", true);
	character->AddComponent<Collider>();
	character->GetComponent<Collider>()->SetShape(colliderShape);

	physics->RegisterEntity(character);
	renderer->RegisterEntity(character);

	//player character container
	std::shared_ptr<Entity> characterContainer = m_EntityManager->CreateEntity<Entity>();
	characterContainer->AddComponent<Transform>();
	characterContainer->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	characterContainer->GetComponent<Transform>()->SetParent(character->GetComponent<Transform>());
	characterContainer->AddComponent<Collider>();
	characterContainer->GetComponent<Collider>()->SetShape(colliderShape);
	characterContainer->GetComponent<Collider>()->isTrigger = true;
	characterContainer->layer = PlayerLayer;
	characterContainer->AddComponent<Character>();
	characterContainer->AddComponent<Billboard>();
	characterContainer->GetComponent<Billboard>()->SetBillboard("./res/textures/ExampleBillboard.DDS", true);
	characterContainer->GetComponent<Character>()->healthBar = characterContainer->GetComponent<Billboard>();
	
	characterContainer->AddComponent<Particles>();

	std::shared_ptr<Entity> soulsMeter2 = m_EntityManager->CreateEntity<Entity>();
	soulsMeter2->AddComponent<HUD>();
	soulsMeter2->GetComponent<HUD>()->setFill(true);
	soulsMeter2->GetComponent<HUD>()->setTextureColor(false);
	soulsMeter2->GetComponent<HUD>()->sethorizontal(true);
	soulsMeter2->GetComponent<HUD>()->setColor(glm::vec4(0.42750f, 0.196f, 0.6588f, 1.0f));
	soulsMeter2->GetComponent<HUD>()->SetHud(0.5f, 0.08f, -0.8f, -0.9f, "./res/purplebg.png");
	renderer->RegisterHUD(soulsMeter2);

	/*
	std::shared_ptr<Entity> soulsMeter = m_EntityManager->CreateEntity<Entity>();
	soulsMeter->AddComponent<HUD>();
	soulsMeter->AddComponent<HUD>()->setFill(true);
	soulsMeter->AddComponent<HUD>()->sethorizontal(true);
	soulsMeter->AddComponent<HUD>()->setColor(glm::vec4(1.0f, 0, 0.0f, 1.0f));
	soulsMeter->GetComponent<HUD>()->SetHud(0.5f, 0.08f, -0.8f, -0.9f, "./res/purplebg.png");
	renderer->RegisterHUD(soulsMeter);
	*/
	

	//characterContainer->GetComponent<Particles>()->Init();
	//characterContainer->GetComponent<Particles>()->SetParticles("./res/textures/particle.DDS", true);
	
	//characterContainer->GetComponent<Character>()->Parts = characterContainer->GetComponent<Particles>();
	characterContainer->AddComponent<HUD>();
	characterContainer->GetComponent<HUD>()->sethorizontal(false);
	characterContainer->GetComponent<HUD>()->setFill(true);
	characterContainer->GetComponent<HUD>()->SetHud(0.19f, 0.3363f, -0.925f, -0.89f, "./res/hpbg2.png");
	characterContainer->GetComponent<Character>()->HUDhealthBar = characterContainer->GetComponent<HUD>();


//	renderer->RegisterPar(characterContainer);
	renderer->RegisterHUD(characterContainer);
	//renderer->RegisterBillboard(characterContainer);
	gameLogic->RegisterEntity(characterContainer);
	physics->RegisterEntity(characterContainer);

	character->GetComponent<Player>()->characterCollider = characterContainer->GetComponent<Collider>();

	
	std::shared_ptr<Entity> hpFrame = m_EntityManager->CreateEntity<Entity>();
	hpFrame->AddComponent<HUD>();
	hpFrame->GetComponent<HUD>()->setFill(false);
	hpFrame->GetComponent<HUD>()->SetHud(0.26f, 0.4602f, -0.96f, -0.95f, "./res/hpbg.png");

	renderer->RegisterHUD(hpFrame); 
	Start->AddComponent<HUD>();
	Start->GetComponent<HUD>()->setFill(false);
	Start->GetComponent<HUD>()->active = true;
	Start->GetComponent<HUD>()->SetHud(2.f, 2.f, -1.f, -1.f, "./res/IntroKomix.png");
	renderer->RegisterHUD(Start);
	character->GetComponent<Player>()->StartScreen = Start->GetComponent<HUD>();
	//character->GetComponent<Character>()->StartScreen = Start->GetComponent<HUD>();

	std::cout << "\n=== Player fully initalized \n";

	//Weapons gui 
	InitializePlayerGUI(renderer, physics, character, gameLogic);

	std::shared_ptr<Entity> pec = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	pec->AddComponent<PlayerEnemyCommunicator>();

	std::shared_ptr<Entity> kratosAxe = m_EntityManager->CreateEntity<Entity>();
	kratosAxe->AddComponent<KratosAxe>();
	kratosAxe->GetComponent<KratosAxe>()->playersCharacter = characterContainer->GetComponent<Character>();
	kratosAxe->GetComponent<KratosAxe>()->pec = pec->GetComponent<PlayerEnemyCommunicator>();
	//pec->GetComponent<PlayerEnemyCommunicator>()->kratosAxe = kratosAxe->GetComponent<KratosAxe>();

	gameLogic->RegisterEntity(kratosAxe);

	// PEC INFO
	std::shared_ptr<Entity> gameLost = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	gameLost->AddComponent<Transform>();
	gameLost->GetComponent<Transform>()->SetLocalPosition(glm::vec3(700, 600, 0));
	gameLost->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.5f, 1.5f, 1.5f));
	gameLost->AddComponent<Text>();
	gameLost->GetComponent<Text>()->text = "YOU HAVE BEEN DEFEATED!";
	gameLost->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(gameLost);
	renderer->RegisterEntity(gameLost);

	std::shared_ptr<Entity> gameWon = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	gameWon->AddComponent<Transform>();
	gameWon->GetComponent<Transform>()->SetLocalPosition(glm::vec3(700, 500, 0));
	gameWon->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.f, 1.f, 1.f));
	gameWon->AddComponent<Text>();
	gameWon->GetComponent<Text>()->text = "THANK YOU FOR PLAYING SOULEATER DEMO!";
	gameWon->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	physics->RegisterEntity(gameWon);
	renderer->RegisterEntity(gameWon);

	gameWon->isActive = false;

	std::shared_ptr<Entity> gameWon2 = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	gameWon2->AddComponent<Transform>();
	gameWon2->GetComponent<Transform>()->SetLocalPosition(glm::vec3(700, 450, 0));
	gameWon2->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.0f, 1.f, 1.f));
	gameWon2->AddComponent<Text>();
	gameWon2->GetComponent<Text>()->text = "NOONE SURVIVED ON THIS ISLAND! YOU WON!";
	gameWon2->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	physics->RegisterEntity(gameWon2);
	renderer->RegisterEntity(gameWon2);

	gameWon2->isActive = false;

	pec->GetComponent<PlayerEnemyCommunicator>()->gameWon = gameWon;
	pec->GetComponent<PlayerEnemyCommunicator>()->gameWon2 = gameWon2;

	//--------------------------------------------------------------------------------------------------------------DEBUG HEALTH <--- TO DELETE \/\/\/
	std::shared_ptr<Entity> debugHealthTitle = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	debugHealthTitle->AddComponent<Transform>();
	debugHealthTitle->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 50, 0));
	debugHealthTitle->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	debugHealthTitle->AddComponent<Text>();
	debugHealthTitle->GetComponent<Text>()->text = "HP:";
	debugHealthTitle->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	//physics->RegisterEntity(debugHealthTitle);
	//renderer->RegisterEntity(debugHealthTitle);

	std::shared_ptr<Entity> debugHealthValue = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	debugHealthValue->AddComponent<Transform>();
	debugHealthValue->GetComponent<Transform>()->SetLocalPosition(glm::vec3(50, 50, 0));
	debugHealthValue->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	debugHealthValue->AddComponent<Text>();
	debugHealthValue->GetComponent<Text>()->text = "100%";
	debugHealthValue->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	//physics->RegisterEntity(debugHealthValue);
	//renderer->RegisterEntity(debugHealthValue);

	std::shared_ptr<Entity> debugAxeTitle = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	debugAxeTitle->AddComponent<Transform>();
	debugAxeTitle->GetComponent<Transform>()->SetLocalPosition(glm::vec3(300, 100, 0));
	debugAxeTitle->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	debugAxeTitle->AddComponent<Text>();
	debugAxeTitle->GetComponent<Text>()->text = "Axe Fill:";
	debugAxeTitle->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	//physics->RegisterEntity(debugAxeTitle);
	//renderer->RegisterEntity(debugAxeTitle);

	std::shared_ptr<Entity> debugAxeValue = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	debugAxeValue->AddComponent<Transform>();
	debugAxeValue->GetComponent<Transform>()->SetLocalPosition(glm::vec3(415, 100, 0));
	debugAxeValue->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	debugAxeValue->AddComponent<Text>();
	debugAxeValue->GetComponent<Text>()->text = "100%";
	debugAxeValue->GetComponent<Text>()->color = glm::vec3(0.6f, 0.2f, 1.0f);

	//physics->RegisterEntity(debugAxeValue);
	//renderer->RegisterEntity(debugAxeValue);

	std::shared_ptr<Entity> soulsTakenTitle = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	soulsTakenTitle->AddComponent<Transform>();
	soulsTakenTitle->GetComponent<Transform>()->SetLocalPosition(glm::vec3(300, 100, 0));
	soulsTakenTitle->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	soulsTakenTitle->AddComponent<Text>();
	soulsTakenTitle->GetComponent<Text>()->text = "Souls:";
	soulsTakenTitle->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	physics->RegisterEntity(soulsTakenTitle);
	renderer->RegisterEntity(soulsTakenTitle);

	std::shared_ptr<Entity> soulsTakenValue = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	soulsTakenValue->AddComponent<Transform>();
	soulsTakenValue->GetComponent<Transform>()->SetLocalPosition(glm::vec3(380, 100, 0));
	soulsTakenValue->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	soulsTakenValue->AddComponent<Text>();
	soulsTakenValue->GetComponent<Text>()->text = "100";
	soulsTakenValue->GetComponent<Text>()->color = glm::vec3(1.0f, 1.0f, 1.0f);

	physics->RegisterEntity(soulsTakenValue);
	renderer->RegisterEntity(soulsTakenValue);


	//--------------------------------------------------------------------------------------------------------------DEBUG HEALTH <--- TO DELETE /\/\/\

	characterContainer->GetComponent<Character>()->gameLostText = gameLost;
	characterContainer->GetComponent<Character>()->healthDebugValue = debugHealthValue->GetComponent<Text>();

	kratosAxe->GetComponent<KratosAxe>()->debugAxeStatus = debugAxeValue->GetComponent<Text>();
	kratosAxe->GetComponent<KratosAxe>()->debugSoulsTaken = soulsTakenValue->GetComponent<Text>();
	kratosAxe->GetComponent<KratosAxe>()->soulsMeter = soulsMeter2->GetComponent<HUD>();

	gameLost->isActive = false;

	//std::shared_ptr<Entity> testingBillboard = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	//testingBillboard->AddComponent<Billboard>();

	character->GetComponent<Player>()->pec = pec->GetComponent<PlayerEnemyCommunicator>();
	std::cout << "\n=== Weapons gui initalized \n";

	// --------- G E N E R A T I N G --- M A P ---------------
	mapGenerator = new MapGenerator(renderer, assetManager, physics,  shadera, grassShader, gameLogic, m_EntityManager, pec);
	mapGenerator->Generate();
	// ------------------------------------------------------
	character->GetComponent<Transform>()->SetLocalPosition(glm::vec3(mapGenerator->mapSizeX/2 * 16, -0.4f, mapGenerator->mapSizeY / 2 * 16));
	//player's weapon ! -> it should be spawned in player's script imo
	std::shared_ptr<Entity> weapon = m_EntityManager->CreateEntity<Entity>();
	weapon->AddComponent<Transform>();
	weapon->GetComponent<Transform>()->SetParent(character->GetComponent<Transform>());
	weapon->GetComponent<Transform>()->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	colliderShape.clear();
	colliderShape.push_back({ -6.0f, -5.0f });
	colliderShape.push_back({ -6.0f, 18.0f });
	colliderShape.push_back({ 6.0f, 18.0f });
	colliderShape.push_back({ 6.0f, -5.0f });

	weapon->AddComponent<Collider>();
	weapon->GetComponent<Collider>()->SetShape(colliderShape);
	weapon->GetComponent<Collider>()->isTrigger = true;
	weapon->AddComponent<Weapon>();
	weapon->GetComponent<Weapon>()->characterContainer = characterContainer;
	weapon->GetComponent<Weapon>()->audioMaster = audioMaster;

	renderer->RegisterEntity(weapon);
	gameLogic->RegisterEntity(weapon);
	physics->RegisterEntity(weapon);


	//-------------GAME MANAGER------------
	std::cout << "\n=== Game manager initalizing \n";
	std::shared_ptr<Entity> gameManager = m_EntityManager->CreateEntity<Entity>();
	std::cout << "- gameManager entity created \n";
	gameManager->AddComponent<GameManager>();
	std::cout << "- GameManager component added \n";
	gameManager->GetComponent<GameManager>()->pec = pec->GetComponent<PlayerEnemyCommunicator>();
	std::cout << "- game manager entity created \n";

	gameLogic->RegisterEntity(gameManager);
	

	//renderer->RegisterEntity(weapon);

	character->GetComponent<Player>()->weapon = weapon->GetComponent<Weapon>();

	
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
	std::cout << "\n=== Sea created\n";
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
	movement->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 1000, 0));
	movement->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
	movement->AddComponent<Text>();
	movement->GetComponent<Text>()->text = "WSAD - move";
	movement->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(movement);
	renderer->RegisterEntity(movement);

	std::shared_ptr<Entity> interaction = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	interaction->AddComponent<Transform>();
	interaction->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 980, 0));
	interaction->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
	interaction->AddComponent<Text>();
	interaction->GetComponent<Text>()->text = "E - use";
	interaction->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(interaction);
	renderer->RegisterEntity(interaction);

	std::shared_ptr<Entity> attack = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	attack->AddComponent<Transform>();
	attack->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 960, 0));
	attack->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
	attack->AddComponent<Text>();
	attack->GetComponent<Text>()->text = "J - attack";
	attack->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(attack);
	renderer->RegisterEntity(attack);

	std::shared_ptr<Entity> roll = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	roll->AddComponent<Transform>();
	roll->GetComponent<Transform>()->SetLocalPosition(glm::vec3(15, 940, 0));
	roll->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.5f, 0.5f, 0.5f));
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
	damageDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(920, 120, 0));
	damageDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	damageDesc->AddComponent<Text>();
	damageDesc->GetComponent<Text>()->text = "Damage (+%)";
	damageDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(damageDesc);
	renderer->RegisterEntity(damageDesc);

	std::shared_ptr<Entity> oldDamage = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldDamage->AddComponent<Transform>();
	oldDamage->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 120, 0));
	oldDamage->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	oldDamage->AddComponent<Text>();
	oldDamage->GetComponent<Text>()->text = "XX";
	oldDamage->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldDamage);
	renderer->RegisterEntity(oldDamage);

	std::shared_ptr<Entity> damageCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	damageCompare->AddComponent<Transform>();
	damageCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 120, 0));
	damageCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	damageCompare->AddComponent<Text>();
	damageCompare->GetComponent<Text>()->text = "->";
	damageCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(damageCompare);
	renderer->RegisterEntity(damageCompare);

	std::shared_ptr<Entity> newDamage = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newDamage->AddComponent<Transform>();
	newDamage->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 120, 0));
	newDamage->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	newDamage->AddComponent<Text>();
	newDamage->GetComponent<Text>()->text = "XX";
	newDamage->GetComponent<Text>()->color = glm::vec3(0.0f, 1.0f, 0.0f);

	physics->RegisterEntity(newDamage);
	renderer->RegisterEntity(newDamage);

	// ATTACK SPEED

	std::shared_ptr<Entity> speedDesc = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	speedDesc->AddComponent<Transform>();
	speedDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(920, 70, 0));
	speedDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	speedDesc->AddComponent<Text>();
	speedDesc->GetComponent<Text>()->text = "Att. Speed (+%)";
	speedDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(speedDesc);
	renderer->RegisterEntity(speedDesc);

	std::shared_ptr<Entity> oldSpeed = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldSpeed->AddComponent<Transform>();
	oldSpeed->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 70, 0));
	oldSpeed->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	oldSpeed->AddComponent<Text>();
	oldSpeed->GetComponent<Text>()->text = "XX";
	oldSpeed->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldSpeed);
	renderer->RegisterEntity(oldSpeed);

	std::shared_ptr<Entity> speedCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	speedCompare->AddComponent<Transform>();
	speedCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 70, 0));
	speedCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	speedCompare->AddComponent<Text>();
	speedCompare->GetComponent<Text>()->text = "->";
	speedCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(speedCompare);
	renderer->RegisterEntity(speedCompare);

	std::shared_ptr<Entity> newSpeed = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newSpeed->AddComponent<Transform>();
	newSpeed->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 70, 0));
	newSpeed->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	newSpeed->AddComponent<Text>();
	newSpeed->GetComponent<Text>()->text = "XX";
	newSpeed->GetComponent<Text>()->color = glm::vec3(0.0f, 1.0f, 0.0f);

	physics->RegisterEntity(newSpeed);
	renderer->RegisterEntity(newSpeed);

	// DURABILITY

	std::shared_ptr<Entity> durDesc = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	durDesc->AddComponent<Transform>();
	durDesc->GetComponent<Transform>()->SetLocalPosition(glm::vec3(920, 20, 0));
	durDesc->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	durDesc->AddComponent<Text>();
	durDesc->GetComponent<Text>()->text = "Durability";
	durDesc->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(durDesc);
	renderer->RegisterEntity(durDesc);

	std::shared_ptr<Entity> oldDur = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	oldDur->AddComponent<Transform>();
	oldDur->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1100, 20, 0));
	oldDur->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	oldDur->AddComponent<Text>();
	oldDur->GetComponent<Text>()->text = "XX";
	oldDur->GetComponent<Text>()->color = glm::vec3(1.0f, 0.0f, 0.0f);

	physics->RegisterEntity(oldDur);
	renderer->RegisterEntity(oldDur);

	std::shared_ptr<Entity> durCompare = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	durCompare->AddComponent<Transform>();
	durCompare->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1150, 20, 0));
	durCompare->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	durCompare->AddComponent<Text>();
	durCompare->GetComponent<Text>()->text = "->";
	durCompare->GetComponent<Text>()->color = glm::vec3(0.8f, 0.8f, 0.8f);

	physics->RegisterEntity(durCompare);
	renderer->RegisterEntity(durCompare);

	std::shared_ptr<Entity> newDur = m_EntityManager->CreateEntity<Entity>(&m_ComponentManager);
	newDur->AddComponent<Transform>();
	newDur->GetComponent<Transform>()->SetLocalPosition(glm::vec3(1180, 20, 0));
	newDur->GetComponent<Transform>()->SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
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
	berserkerMode->GetComponent<Transform>()->SetLocalPosition(glm::vec3(825, 900, 0));
	berserkerMode->GetComponent<Transform>()->SetLocalScale(glm::vec3(1.5f, 1.5f, 1.5f));
	berserkerMode->AddComponent<Text>();
	berserkerMode->GetComponent<Text>()->text = "BERSERKER!";
	berserkerMode->GetComponent<Text>()->color = glm::vec3(0.6f, 0.2f, 1.0f);

	physics->RegisterEntity(berserkerMode);
	renderer->RegisterEntity(berserkerMode);

	berserkerMode->isActive = false;

	player->GetComponent<Player>()->berserkerModeText = berserkerMode;
}