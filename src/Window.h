#pragma once
#include "Core.h"
#include "IEventListener.h"

class Window : public IEventListener
{
public:
	Window();
	~Window();

	void Init(std::string name, int width, int height);
	static std::shared_ptr<Window> GetInstance();

	GLFWwindow* GetMWindow();

private:
	static std::shared_ptr<Window> m_Instance;

	int m_Width;
	int m_Height;
	std::string m_Name;
	GLFWwindow* m_Window;
	void OnEvent(Event& e) override;
	bool DebugEvent(LogEvent& e);
};