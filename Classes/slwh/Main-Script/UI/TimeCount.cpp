#include "TimeCount.h"

const float TimeCount::UPDATE_TIME = 1.0f;
TimeCount::TimeCount():
m_num(NULL),
m_nowTime(_ZERO),
m_bIsRun(false)
{
}


TimeCount::~TimeCount()
{
}


void TimeCount::Start()
{

	m_num = dynamic_cast<TextAtlas*>(m_node);
	CC_ASSERT(NULL != m_num);
	m_num->setOpacity(_ZERO);

}

void TimeCount::TimeUpdate(float t)
{
	if (--m_nowTime < _ZERO)
	{
		m_nowTime = _ZERO;
		StopPlay();
		return;
	}
	
	std::stringstream str;
	str << m_nowTime;
	m_num->setString(str.str());

	if (m_nowTime == _ZERO)
	{
		_PLAY_MUSCI_EFF(_TIME0);
	}
	else
	{
		_PLAY_MUSCI_EFF(_TIME1);
		
	}
}


//开启倒计时
void TimeCount::StartPlay(int time)
{
	if (NULL == m_node)
		return;

	if (m_bIsRun)
		return;
	m_nowTime = time;
	m_bIsRun = true;

	std::stringstream str;
	str << m_nowTime;
	m_num->setString(str.str());
	m_num->runAction(FadeIn::create(0.5f));
	schedule(schedule_selector(TimeCount::TimeUpdate),UPDATE_TIME);
}


//停止倒计时
void TimeCount::StopPlay()
{

	if (NULL == m_node)
		return;

	if (!m_bIsRun)
		return;

	m_bIsRun = false;
	m_num->runAction(FadeOut::create(0.5f));
	unschedule(schedule_selector(TimeCount::TimeUpdate));
}


