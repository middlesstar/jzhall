#include "df/Timer.h"
#include "TimestampManager_SHZ.h"

SHZTimestampManager gTimestampManager_SHZ;


SHZTimestampManager::SHZTimestampManager()
{
	time_local_ = 0;
	current_tick_ = 0;
	time_trad_ = 0;
	server_time_ = 0;
}

uint32 SHZTimestampManager::getTimeCheck() const
{
	return CoTimer::getCurrentTime();
}

void SHZTimestampManager::timeCheck(uint32 clientTime, uint32 serverTime)
{
	if (clientTime != 0)
		time_local_ = clientTime;
	else
		time_local_ = getTimeCheck();

	clientTime = getTimeCheck();
	if (clientTime > time_local_)
		time_trad_ = (clientTime - time_local_)/2;
	else
		time_trad_ = 0;

	server_time_ = serverTime;
}

uint32 SHZTimestampManager::get_unified_time() const
{
	return time_trad_;
}

uint32 SHZTimestampManager::get_trad() const
{
	return time_trad_;
}

uint32 SHZTimestampManager::get_time_difference(uint32 packet_time)
{
	uint32 myTrad1 = packet_time + time_trad_;
	if (myTrad1 < 0)
		myTrad1 = 0;
	if (myTrad1 > MAX_DIFFERENCE_SHZ)
		myTrad1 = MAX_DIFFERENCE_SHZ;
	return myTrad1;
}


void SHZTimestampManager::tick(float dt)
{
	current_tick_ += dt;
}