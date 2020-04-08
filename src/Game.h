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
	std::unique_ptr<Window> m_Window;
};