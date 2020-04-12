#pragma once
#include "Core.h"
#include "IEventListener.h"

class Window : public IEventListener
{
public:
	Window();
	~Window();

	static Window m_Instance;

	void Init(std::string name, int width, int height);

	GLFWwindow* GetMWindow();

private:
	int m_Width;
	int m_Height;
	std::string m_Name;
	GLFWwindow* m_Window;
	void OnEvent(Event& e) override;
	bool DebugEvent(LogEvent& e);
};