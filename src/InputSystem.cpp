#include "InputSystem.h"
#include "Window.h"

std::set<std::pair<int, int>> InputSystem::m_KeyboardInputQueue{ { 0,0 } };
ControllerInput InputSystem::m_ControllerInputQueue;

void RegisterKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputSystem::AddInput(key, action);
}

InputSystem::InputSystem()
{
	glfwSetKeyCallback(Window::GetInstance()->GetMWindow(), RegisterKeys);
}

void InputSystem::AddInput(int input, int action)
{
	m_KeyboardInputQueue.emplace(std::make_pair(input, action));
}

InputSystem::~InputSystem()
{
}

void InputSystem::Update() const
{
	for (std::shared_ptr<Entity> entity : m_Entities)
	{
		std::shared_ptr<InputHandler> handler = entity->GetComponent<InputHandler>();
		handler->m_Keyboard = m_KeyboardInputQueue;
	}
}

void InputSystem::LateUpdate() const
{
	m_KeyboardInputQueue.clear();
}
