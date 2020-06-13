#pragma once
#include "Core.h"
#include "SystemManager.h"
#include "Window.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "AssetManager.h"
#include "GameLogic.h"
#include "InputSystem.h"
#include <fstream>
#include <iostream>
#include "Model.h"
#include "Scripts/MapGenerator.h"


class Game
{
public:
	Game();
	~Game();

	void Init();
	void Run();
	void EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem, MapGenerator* mapGenerator);
	Model * FindModelByName(Model* [], std::string name);
private:
	void InitializeWeapons(AssetManager* assetManager);
	void InitializeBasicGUI(Renderer* renderer, Physics* physics);
	void InitializePlayerGUI(Renderer* renderer, Physics* physics, std::shared_ptr<Entity> player, GameLogic* gameLogic);

	std::shared_ptr<Window> m_Window;

	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<ComponentManager> m_ComponentManager;
	SystemManager m_SystemManager;
};