#pragma once
static class TimeCustom
{
public:
	TimeCustom();
	~TimeCustom();

	static double GetDeltaTime();
	static void RunTimer();
	static double GetTime();
private:
	static double m_Time;
	static double m_DeltaTime;
};

