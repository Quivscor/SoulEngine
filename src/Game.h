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
class Game
{
public:
	Game();
	~Game();

	void Init();
	void Run();
	void EntitiesInit(AssetManager* assetManager, Renderer* renderer, Physics* physics, GameLogic* gameLogic, std::shared_ptr<Entity> inputSystem);
	void LoadMap( Renderer*, AssetManager*, Physics*);
	Model * FindModelByName(Model* [], std::string name);
private:
	std::shared_ptr<Window> m_Window;

	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<ComponentManager> m_ComponentManager;
	SystemManager m_SystemManager;
};