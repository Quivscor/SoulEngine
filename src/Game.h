#pragma once
#include "Core.h"
#include "SystemManager.h"
#include "Window.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "AssetManager.h"
#include <fstream>
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Run();
	void LoadMap(int, int, Renderer*, AssetManager*, Physics*);
private:
	std::shared_ptr<Window> m_Window;

	EntityManager m_EntityManager;
	ComponentManager m_ComponentManager;
	SystemManager m_SystemManager;
};