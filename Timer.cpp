#include "Timer.h"

Timer::Timer()
{
	// Check to see if this system supports high performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	if (frequency == 0)
	{
		throw runtime_error("Could not initialize Timer");
	}

	ticksPerMs = (float)(frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

Timer::~Timer()
{
}

void Timer::Update()
{
	INT64 currentTime;
	float timeDifference;

	//Query the current time
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	//Difference in time since the last time-query 
	timeDifference = (float)(currentTime - startTime);
	//Time difference over the timer speed resolution give frameTime
	frameTime = timeDifference / ticksPerMs;
	//Restart the timer
	startTime = currentTime;
}

float Timer::GetTime()
{
	return frameTime;
}
