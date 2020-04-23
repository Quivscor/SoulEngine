#include "InputHandler.h"
#include "InputSystem.h"

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::CheckKey(int key, int action)
{
	auto it = m_Keyboard.find({ key, action });

	if (it == m_Keyboard.end())
		return false;
	else
		return true;
}

bool InputHandler::CheckLastFrameKey(int key, int action)
{
	auto it = m_KeyboardLastFrame.find({ key, action });

	if (it == m_KeyboardLastFrame.end())
		return false;
	else
		return true;
}

bool InputHandler::GetKeyUp(int key)
{
	return InputHandler::CheckKey(key, GLFW_RELEASE );
}

bool InputHandler::GetKeyDown(int key)
{
	return InputHandler::CheckKey(key, GLFW_PRESS);
}

bool InputHandler::GetKeyRepeat(int key)
{
	return (InputHandler::CheckKey(key, GLFW_PRESS) && InputHandler::CheckLastFrameKey(key, GLFW_PRESS));
}
