#include "GlobalTimer.h"
#include "Player/Player.h"

#define SCHEDULE CCDirector::sharedDirector()->getScheduler()

GlobalTimer* GlobalTimer::m_instance = nullptr;
GlobalTimer::GlobalTimer()
{
}

GlobalTimer::~GlobalTimer()
{
	SCHEDULE->unscheduleSelector(
		schedule_selector(GlobalTimer::GlobalUpdate), this);
}

// 启动全局定时器
void GlobalTimer::StarPlay()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(GlobalTimer::GlobalUpdate),this,1.f / 60.f,false);

}

// 停止
void GlobalTimer::StopPlay()
{
	CC_SAFE_DELETE(m_instance);
}

// 暂停
void GlobalTimer::Pause()
{
	SCHEDULE->pauseTarget(m_instance);
}

// 恢复  
void GlobalTimer::Resume()
{
	SCHEDULE->resumeTarget(m_instance);
}

// 得到全局定时器对象
GlobalTimer* GlobalTimer::Getinstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new  GlobalTimer();
	}
	return m_instance;
}

void GlobalTimer::GlobalUpdate(float t)
{
	//抢福袋逻辑		
	static float fudaitime = 0.f;			// 时间累加	
	if ( (fudaitime += t) >= 1.f )		// 每1秒执行一次
	{
		auto data = Player::getInstance()->getHallData();
		if (data.fudaitime > 0)
		{
			data.fudaitime--;
			Player::getInstance()->setHallData(data);
		}	
		fudaitime = 0.f;
	}
	



}
