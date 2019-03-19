#ifndef SHZ_TimestampManager_H_
#define SHZ_TimestampManager_H_

#include "shuihu/GameHeader_SHZ.h"

class SHZTimestampManager
{
public:
	SHZTimestampManager();

	uint32 getTimeCheck() const;
	void timeCheck(uint32 clientTime, uint32 serverTime);
	
	uint32 get_unified_time() const ;
	uint32 get_trad() const ;
	uint32 get_time_difference(uint32 packet_time);

	void tick(float dt);
private:
	uint32 time_local_;
	uint32 time_trad_;
	uint32 server_time_;
	double current_tick_;

};

extern SHZTimestampManager gTimestampManager_SHZ;
#endif //_TimestampManager_H_