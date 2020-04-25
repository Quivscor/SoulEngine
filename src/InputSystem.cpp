#include "InputSystem.h"
#include "Window.h"
#include <iostream>

std::set<std::pair<int, int>> InputSystem::m_KeyboardInputQueue{ { 0,0 } };
std::set<std::pair<int, int>> InputSystem::m_KeyboardInputQueueLastFrame{ { 0,0 } };
std::vector<int> InputSystem::m_ReleaseQueue{ 0 };
ControllerInput InputSystem::m_ControllerInputQueue;

void RegisterKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputSystem::AddInput(key, action);
	if (action == GLFW_RELEASE)
	{
		InputSystem::AddRelease(key);
	}
}

InputSystem::InputSystem()
{
	glfwSetKeyCallback(Window::GetInstance()->GetMWindow(), RegisterKeys);
}

void InputSystem::AddInput(int input, int action)
{
	m_KeyboardInputQueue.emplace(std::make_pair(input, action));
}

void InputSystem::AddRelease(int input)
{
	m_ReleaseQueue.emplace_back(input);
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
		handler->m_KeyboardLastFrame = m_KeyboardInputQueueLastFrame;
	}
}

void InputSystem::LateUpdate() const
{
	m_KeyboardInputQueueLastFrame = m_KeyboardInputQueue;
	for (int input : m_ReleaseQueue)
	{
		if (m_KeyboardInputQueue.erase({ input, GLFW_PRESS }) == 0)
		{
			m_KeyboardInputQueue.erase({ input, GLFW_RELEASE });
		}
	}
	m_ReleaseQueue.clear();
}
