#include "Window.h"

Window::Window()
{
	SetEventCallback(BIND_EVENT_FN(Window::OnEvent));
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

void Window::Init(std::string name, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "ERROR :: Window was not created.";
	}
	glfwMakeContextCurrent(m_Window);
}

void Window::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<LogEvent>(BIND_EVENT_FN(Window::DebugEvent));
}

bool Window::DebugEvent(LogEvent& e)
{
	std::cout << "EVENT :: Logged Window event. Message: " + e.ToString();
	return true;
}
