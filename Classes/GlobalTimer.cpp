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

// ����ȫ�ֶ�ʱ��
void GlobalTimer::StarPlay()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(GlobalTimer::GlobalUpdate),this,1.f / 60.f,false);

}

// ֹͣ
void GlobalTimer::StopPlay()
{
	CC_SAFE_DELETE(m_instance);
}

// ��ͣ
void GlobalTimer::Pause()
{
	SCHEDULE->pauseTarget(m_instance);
}

// �ָ�  
void GlobalTimer::Resume()
{
	SCHEDULE->resumeTarget(m_instance);
}

// �õ�ȫ�ֶ�ʱ������
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
	//�������߼�		
	static float fudaitime = 0.f;			// ʱ���ۼ�	
	if ( (fudaitime += t) >= 1.f )		// ÿ1��ִ��һ��
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
