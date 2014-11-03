#include "CTimer.h"

CTimer::CTimer()
{
}
	
void CTimer::Tick(double fLockFPS)
{
	if(fLockFPS!=0.0f)
	{
		float timeElapsed=0.0f;
		while((double)(clock()-clocks)/(double)(CLOCKS_PER_SEC)<1.0f/fLockFPS)
		{
		}
		clocks=clock();
		mTimeElapsed=1.0f/fLockFPS;
	}
	else
	{
		mTimeElapsed=(double)(clock()-clocks)/(double)(CLOCKS_PER_SEC);
		clocks=clock();
	}

	static double timeElapsedFR=0;
	static int frameRate=0;
	timeElapsedFR+=mTimeElapsed;
	++frameRate;
	if(timeElapsedFR>=1.0f)
	{
		mFrameRate=(double)frameRate;
		timeElapsedFR=0.0f;
		frameRate=0;
	}
}
	
double CTimer::GetTimeElapsed()
{
	return mTimeElapsed;
}

double CTimer::GetFrameRate()
{
	return mFrameRate;
}

STimer::STimer(){
	clocks=0;
}
	
void STimer::Start(){
	clocks=clock();
}

double STimer::Time(){
	return (double)(clock()-clocks)/(double)(CLOCKS_PER_SEC);
}
