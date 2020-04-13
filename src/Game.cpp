#include "Game.h"

//TO DELETE:
#include "Material.h"
#include "Renderer.h"
#include "Physics.h"

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

	//Creating simple transform
	Transform* transform = new Transform();

	transform->position = glm::vec3(0.33f, 0.22f, 0.0f);
	transform->rotation = glm::vec3(0.0f, 0.0f, 45.0f);
	transform->scale = glm::vec3(0.5f, 0.75f, 1.0f);

	//Creating systems
	Renderer* renderer = new Renderer(1, shader);
	Physics* physics = new Physics(2);
	physics->transform = transform;

	//---------------------------------------------------------------------------------

	while (true)
	{
		transform->rotation = glm::vec3(0.0f, 0.0f, (GLfloat)glfwGetTime() * 10.0f);

		physics->Update();
		renderer->Update();
		renderer->DrawSquare(transform);

		physics->LateUpdate();
		renderer->LateUpdate();
	}
}