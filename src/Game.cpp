#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"

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

	//Creating simple material
	Material* material = new Material(shader);

	//Creating renderer
	Renderer* renderer = new Renderer(1, shader);

	//---------------------------------------------------------------------------------

	while (true)
	{
		renderer->Update();
		renderer->DrawSquare();

		renderer->LateUpdate();
	}
}