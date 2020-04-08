#include "Game.h"

Game::Game() {}

Game::~Game() 
{
	glfwTerminate();
}

void Game::Init()
{
	m_Window = std::make_unique<Window>();
	m_Window->Init("SoulEater", 640, 480);
}

void Game::Run()
{
	while (true)
	{
		LogEvent e("Hello events!");
		m_Window->EventCallback(e);
	}
}