#include "Time.h"
#include "GLFW/glfw3.h"
#include <iostream>

double Time::m_Time = 0;
double Time::m_DeltaTime = 0;

Time::Time()
{
}

Time::~Time()
{
}
double Time::GetTime()
{
	
	return Time::m_Time;
}
double Time::GetDeltaTime()
{
	return Time::m_DeltaTime;
}

void Time::RunTimer()
{
	Time::m_DeltaTime = glfwGetTime() - Time::m_Time;
	Time::m_Time = glfwGetTime();
}

