// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "Core.h"
#include "Game.h"

int main(int, char**)
{
	Game* game = new Game();
	game->Init();
	game->Run();

    return 0;
}