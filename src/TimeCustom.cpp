#include "TimeCustom.h"
#include "GLFW/glfw3.h"
#include <iostream>

double TimeCustom::m_Time = 0;
double TimeCustom::m_DeltaTime = 0;

TimeCustom::TimeCustom()
{
}

TimeCustom::~TimeCustom()
{
}
double TimeCustom::GetTime()
{
	
	return TimeCustom::m_Time;
}
double TimeCustom::GetDeltaTime()
{
	return TimeCustom::m_DeltaTime;
}

void TimeCustom::RunTimer()
{
	if (TimeCustom::m_Time == 0)
		TimeCustom::m_Time = glfwGetTime();
	TimeCustom::m_DeltaTime = glfwGetTime() - TimeCustom::m_Time;
	TimeCustom::m_Time = glfwGetTime();
}

