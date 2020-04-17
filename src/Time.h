#pragma once
static class Time
{
public:
	Time();
	~Time();

	static double GetDeltaTime();
	static void RunTimer();

private:
	static double m_Time;
	static double m_DeltaTime;
};

