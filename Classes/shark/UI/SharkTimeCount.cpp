#include "SharkTimeCount.h"

const float SharkTimeCount::UPDATE_TIME = 1.0f;
SharkTimeCount::SharkTimeCount() :
m_battleTime(NULL),
m_betTime(NULL),
timeIndex(0),
m_nowTime(0),
betTime(NULL),
battleTime(NULL),
flag(true)
{
}


SharkTimeCount::~SharkTimeCount()
{
}


void SharkTimeCount::setContent()
{

	m_betTime = dynamic_cast<TextAtlas*>(_seekNodeByName(m_pNode,"betLabel"));
	CC_ASSERT(NULL != m_betTime);

	betTime = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, "betTime"));
	CC_ASSERT(NULL != betTime);

}

void SharkTimeCount::TimeUpdate(float t)
{
	if (--m_nowTime < 1)
	{
		HideTime();
		m_nowTime = 0;
		StopPlay();
		return;
	}
	std::stringstream str;
	str << m_nowTime;

	m_betTime->setString(str.str());


	if (m_nowTime == 1)
	{
		_PLAY_SOUND_SHARK(LASTTIME);

	}
	else
	{
		if (m_nowTime < 6)
		{
			_PLAY_SOUND_SHARK(TIMEOVER);
		}		
		else{}
	}
}


//开启倒计时
void SharkTimeCount::StartPlay(int time,int index)
{
	if (NULL == m_pNode)
		return;


	showTime();
	timeIndex = index;
	m_nowTime = time;

	std::stringstream str;
	str << m_nowTime;
	m_betTime->setString(str.str());
	if (index == 1)
	{
		betTime->setVisible(true);
		betTime->setPosition(POSONE);
	}
		
	else if(index == 2)
	{
		betTime->setVisible(false);
		betTime->setPosition(POSTWO);
		flag = false;
	}	
	
	schedule(schedule_selector(SharkTimeCount::TimeUpdate), UPDATE_TIME);
}


//停止倒计时
void SharkTimeCount::StopPlay()
{

	if (NULL == m_pNode)
		return;

	unschedule(schedule_selector(SharkTimeCount::TimeUpdate));
}

//切换倒计时显示框的位置
void SharkTimeCount::changeTimeCount()
{
	if (!flag)
	{
		betTime->setVisible(true);
		betTime->setPosition(POSONE);
	}
	else
	{
		betTime->setPosition(POSTWO);
	}
}


//设置倒计时框是否显示
void SharkTimeCount::showTime()
{
	betTime->setVisible(true);
}
void SharkTimeCount::HideTime()
{
	betTime->setVisible(false);
}


