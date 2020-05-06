#pragma once
static class Time
{
public:
	Time();
	~Time();

	static double GetDeltaTime();
	static void RunTimer();
	static double GetTime();
private:
	static double m_Time;
	static double m_DeltaTime;
};

