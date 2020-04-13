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
	Renderer* renderer = new Renderer(shader);
	Physics* physics = new Physics();

	//setting values (it should be exectuted by using events)
	physics->transform = transform;

	//---------------------------------------------------------------------------------

	while (true)
	{
		material->SetColor(glm::vec3(sin((GLfloat)glfwGetTime()), 1.0f, cos((GLfloat)glfwGetTime() * 0.24f)));
		transform->rotation = glm::vec3(0.0f, (GLfloat)glfwGetTime() * 5.0f, (GLfloat)glfwGetTime() * 10.0f);

		physics->Update();
		renderer->Update();
		renderer->DrawCube(transform, material);

		physics->LateUpdate();
		renderer->LateUpdate();
	}
}