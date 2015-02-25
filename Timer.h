#pragma once
#include <windows.h>
#include <stdexcept>

class Timer
{
private:

	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;

public:

	Timer();
	~Timer();

	void Update();

	float GetTime();
};

