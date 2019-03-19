#include "cocos2d.h"
#include "df/types.h"
#include "Timer.h"
USING_NS_CC;
//////////////////////////////////////////////////////////////////////////
unsigned long CoTimer::getCurrentTime()
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	uint64 times = ((uint64)tm.tv_sec * 1000 + tm.tv_usec / 1000);
	times <<= 32;
	times >>= 32;
	return times;
}

//////////////////////////////////////////////////////////////////////////
CoTimer::CoTimer(int delay)
{
	init(delay);
}

void CoTimer::init(int delay)
{
	mStart = CoTimer::getCurrentTime();
	mDelay = delay;
}

bool CoTimer::isTimeUp() const
{
	return CoTimer::getCurrentTime() - mStart >= mDelay;
}


unsigned long CoTimer::getElapsed() const
{
	return CoTimer::getCurrentTime() - mStart;
}