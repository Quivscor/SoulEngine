#include "Window.h"

std::shared_ptr<Window> Window::m_Instance = 0;

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
	m_Width = width;
	m_Height = height;
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

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR :: Failed to initialize GLAD" << std::endl;
	}
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

std::shared_ptr<Window> Window::GetInstance()
{
	if (Window::m_Instance == NULL)
	{
		Window::m_Instance = std::make_shared<Window>();
	}

	return Window::m_Instance;
}

GLFWwindow* Window::GetMWindow()
{
	return m_Window;
}
