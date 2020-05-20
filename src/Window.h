#pragma once
#include "Core.h"
#include "IEventListener.h"
#include "Camera.h"

class Window : public IEventListener
{
public:
	friend class Camera;

	Window();
	~Window();

	void Init(std::string name, int width, int height);
	static std::shared_ptr<Window> GetInstance();

	 GLFWwindow* GetMWindow();
	int m_Width;
	int m_Height;
private:
	static std::shared_ptr<Window> m_Instance;


	std::string m_Name;
	GLFWwindow* m_Window;
	void OnEvent(Event& e) override;
	bool DebugEvent(LogEvent& e);
};