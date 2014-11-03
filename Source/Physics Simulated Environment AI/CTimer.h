#ifndef _CTIMER_H_
#define _CTIMER_H_

#include <time.h>

class CTimer{
public:
	CTimer();

	void Tick(double fLockFPS=0.0f);
	double GetTimeElapsed();
	double GetFrameRate();

private:
	clock_t clocks;
	double mTimeElapsed;
	double mFrameRate;
};


class STimer
{
public:
	STimer();

	void Start();
	double Time();

private:
	clock_t clocks;
};

#endif