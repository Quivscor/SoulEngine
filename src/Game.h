#pragma once
#include "Core.h"
#include "Window.h"

class Game
{
public:
	Game();
	~Game();

	void Init();
	void Run();
private:
	std::shared_ptr<Window> m_Window;
};