#include "SharkGoldHeap.h"

#ifndef _ZERO
#define _ZERO 0
#endif

SharkGoldHeap::SharkGoldHeap() :
m_bIsRunning(false),
m_spriteArr(nullptr),
m_targetindex(_ZERO),
m_nowindex(_ZERO),
m_maxgold(_ZERO),
m_text(nullptr),
m_light(nullptr)
{
}

bool SharkGoldHeap::init(int maxgold)
{
	if (false == Node::init())
	{
		return false;
	}

	m_text = Text::create(a_u8(""),a_u8("Î¢ÈíÑÅºÚ"),20);
	CC_ASSERT(m_text != nullptr);
	addChild(m_text);
	m_text->setPosition(Vec2(_ZERO,7));

	m_maxgold = maxgold;
	m_spriteArr = new Sprite*[maxgold];
	for (size_t i = _ZERO; i < maxgold; i++)
	{
		m_spriteArr[i] = Sprite::create(Path_PNG_Gold_Root);
		CC_ASSERT(m_spriteArr[i] != nullptr);
		addChild(m_spriteArr[i]);
		m_spriteArr[i]->setPosition(Vec2(_ZERO, i * 7));		//Í¼Æ¬¿í¶ÈÎª7
		m_spriteArr[i]->setVisible(false);
	}

	m_light = Sprite::create(Path_PNG_Gold_Light);
	CC_ASSERT(m_light != nullptr);
	addChild(m_light);
	m_light->setVisible(false);

	return true;
}

SharkGoldHeap::~SharkGoldHeap()
{
	delete[] m_spriteArr;
}

void SharkGoldHeap::Running(float deltaTime)
{

	//½ð±Ò¶ÑµÄ¹â
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

void SharkGoldHeap::StartPlay(int index, longlong gold)
{
	if (m_targetindex != index)
	{
		m_targetindex = index;
		if (!m_bIsRunning)
		{
			m_bIsRunning = true;
			schedule(schedule_selector(SharkGoldHeap::Running), 0.05f);
		}
	}

	char buf[256];
	if (gold >= 1000)
	{

		auto gameInfo = GameInfo::GetInstance();
		int roomNum = gameInfo->getroomIndex();
		if (roomNum == 2)
		{
			sprintf(buf, "%ldk", gold / 1000);
		}
		else if (roomNum == 1)
		{
			sprintf(buf, "%.1fK", 1.f * gold / 1000);
		}
		else if (roomNum == 0)
		{
			sprintf(buf, "%.2fK", 1.f * gold / 1000);
		}
		//sprintf(buf, "%.2fK", 1.f*gold / 1000);
	}
	else
	{
		sprintf(buf, "%ld", gold);
	}
	m_text->setVisible(true);
	m_text->setString(buf);
	//m_light->setVisible(true);

}

void SharkGoldHeap::StartPlay(float per, longlong gold)
{
	CC_ASSERT( per >= _ZERO && per <= 1.f);

	StartPlay((int)(m_maxgold * per),gold);
}

SharkGoldHeap* SharkGoldHeap::create(int maxgold)
{ 
	SharkGoldHeap *pRet = new(std::nothrow) SharkGoldHeap();
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

void SharkGoldHeap::StopPlay()
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
	unschedule(schedule_selector(SharkGoldHeap::Running));
}


