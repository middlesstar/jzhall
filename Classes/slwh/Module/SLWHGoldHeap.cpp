#include "SLWHGoldHeap.h"
#include "slwh/ResourcePath.h"

#ifndef _ZERO
#define _ZERO 0
#endif

SLWH_GoldHeap::SLWH_GoldHeap() :
m_bIsRunning(false),
m_spriteArr(nullptr),
m_targetindex(_ZERO),
m_nowindex(_ZERO),
m_maxgold(_ZERO),
m_text(nullptr),
m_light(nullptr)
{
}

bool SLWH_GoldHeap::init(int maxgold)
{
	if (false == Node::init())
	{
		return false;
	}

	m_text = Text::create(a_u8(""),a_u8(""),20);
	CC_ASSERT(m_text != nullptr);
	addChild(m_text);
	m_text->setPosition(Vec2(_ZERO,7));

	m_maxgold = maxgold;
	m_spriteArr = new Sprite*[maxgold];
	for (size_t i = _ZERO; i < maxgold; i++)
	{
		m_spriteArr[i] = Sprite::create(SLWH_GoldHeap_GOLD);
		CC_ASSERT(m_spriteArr[i] != nullptr);
		addChild(m_spriteArr[i]);
		m_spriteArr[i]->setPosition(Vec2(_ZERO, i * 7));		//图片宽度为7
		m_spriteArr[i]->setVisible(false);
	}

	m_light = Sprite::create(Path_PNG_Gold_Light);
	CC_ASSERT(m_light != nullptr);
	addChild(m_light);
	m_light->setVisible(false);

	return true;
}

SLWH_GoldHeap::~SLWH_GoldHeap()
{
	delete[] m_spriteArr;
}

void SLWH_GoldHeap::Running(float deltaTime)
{

	//金币堆的光
	if (m_nowindex == _ZERO)
	{
		m_light->setVisible(false);
	}
	else
	{
		m_light->setVisible(true);
		Vec2 pos = m_light->getPosition();
		if (pos.y < (m_nowindex - 2) * 7)
		{
			pos += Vec2(_ZERO, 10);
		}
		else
		{
			pos = Vec2(_ZERO, _ZERO);
		}
		m_light->setPosition(pos);

	}
	

	if (m_nowindex > m_targetindex)
	{
		m_text->setPosition(Vec2(_ZERO, (m_nowindex + 1) * 7));
		m_nowindex--;
		m_spriteArr[m_nowindex]->setVisible(false);
		
	}
	else if (m_nowindex < m_targetindex)
	{
		m_spriteArr[m_nowindex]->setVisible(true);
		m_nowindex++;
		m_text->setPosition(Vec2(_ZERO, (m_nowindex + 1) * 7));
	}

}

void SLWH_GoldHeap::StartPlay(int index, longlong gold)
{
	

	if (index > m_maxgold) index = m_maxgold;
	if (index < _ZERO) index = _ZERO;


	if (m_targetindex != index)
	{
		m_targetindex = index;
		if (!m_bIsRunning)
		{
			m_bIsRunning = true;
			schedule(schedule_selector(SLWH_GoldHeap::Running), 0.05f);
		}
	}


	char buf[256];
	if (gold > 1000)
	{
		slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
		switch (info->wType)
		{
		case 0:
			sprintf(buf, "%.2fK", (float)gold / 1000);
			break;
		case 1:
			sprintf(buf, "%.1fK", (float)gold / 1000);
			break;
		case 2:
			sprintf(buf, "%.0fK", (float)gold / 1000);
			break;
		default:
			break;
		}
		
	}
	else
	{
		sprintf(buf, "%lld", gold);
	}
	m_text->setVisible(true);
	m_text->setString(buf);
	//m_light->setVisible(true);

}

void SLWH_GoldHeap::StartPlay(float per, longlong gold)
{
	StartPlay((int)(m_maxgold * per),gold);
}

SLWH_GoldHeap* SLWH_GoldHeap::create(int maxgold)
{ 
	SLWH_GoldHeap *pRet = new(std::nothrow) SLWH_GoldHeap();
	if (pRet && pRet->init(maxgold))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
}

void SLWH_GoldHeap::StopPlay()
{
	while (m_nowindex)
	{
		--m_nowindex;
		m_spriteArr[m_nowindex]->setVisible(false);
	}
	m_text->setPosition(Vec2(_ZERO, 7));
	m_text->setVisible(false);
	m_light->setVisible(false);
	m_bIsRunning = false;
	unschedule(schedule_selector(SLWH_GoldHeap::Running));
}


