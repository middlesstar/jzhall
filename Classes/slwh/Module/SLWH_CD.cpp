#include "SLWH_CD.h"
#include "slwh/ResourcePath.h"

#ifndef _ZERO
#define _ZERO	0
#endif


#define		CD_OFFSET							10


//彩带运动属性
#define		MIN_SPEED							300
#define		MAX_SPEED							800
#define		LR_SPEED_MIN						50
#define		LR_SPEED_MAX						150


SLWH_CD* SLWH_CD::m_instance = nullptr;
SLWH_CD::SLWH_CD() :
m_bIsOpen(false)
{
}


SLWH_CD::~SLWH_CD()
{
}



SLWH_CD* SLWH_CD::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new SLWH_CD();
		m_instance->init();
	}

	return m_instance;
}


void SLWH_CD::StartPlay()
{
	Scene* main = Director::getInstance()->getRunningScene();
	SLWH_CD* This = GetInstance();
	if (!main->getChildByName("CDFallen"))
	{
		main->addChild(This, 200);
	}

	This->m_bIsOpen = true;
	This->scheduleUpdate();
}


void SLWH_CD::StopPlay()
{
	SLWH_CD* This = GetInstance();
	This->m_bIsOpen = false;
}


void SLWH_CD::update(float deltaTime)
{
	if (m_bIsOpen)
	{
		CreatOne();
	}


	for (Vector<SLWH_OneCD*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		SLWH_OneCD* cd = (*itor);
		Vec2 v2 = cd->getPosition();
		if (cd->bIsleft)
		{
			v2 += Vec2(-random(LR_SPEED_MIN, LR_SPEED_MAX) * deltaTime, -cd->speed * deltaTime);
		}
		else
		{
			v2 += Vec2(random(LR_SPEED_MIN, LR_SPEED_MAX) * deltaTime, -cd->speed * deltaTime);
		}

		cd->setPosition(v2);

		if (random(_ZERO, 50) == _ZERO)
		{
			cd->bIsleft = !cd->bIsleft;
		}

		cd->setRotation(cd->getRotation() + cd->rotateSpeed * deltaTime);

		if (v2.y < -CD_OFFSET)
		{
			removeChild(cd, true);
			itor = m_list.erase(itor);
		}
		else
		{
			++itor;
		}
	}

}



bool SLWH_CD::init()
{

	m_visibleSize = Director::getInstance()->getWinSize();
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH);
	for (size_t i = _ZERO; i < CD_MAX; i++)
	{
		std::stringstream str;
		str << Path_PNG_CD_Root << i + 1 << ".png";
		m_texture2D[i] = str.str();
	}

	setName("CDFallen");
	scheduleUpdate();

	return true;
}


void SLWH_CD::CreatOne()
{
	int x = random(_ZERO, CD_MAX - 1);
	SLWH_OneCD *cd = SLWH_OneCD::create(m_texture2D[x]);

	//起始位置
	cd->setPosition(Vec2(random(0.f, m_visibleSize.x), m_visibleSize.y + CD_OFFSET));

	//缩放0.8我觉得比较好看
	cd->setScale(0.8f);
	//下落速度
	cd->speed = random(MIN_SPEED, MAX_SPEED);
	if (random(0, 1) == 0)
	{
		cd->rotateSpeed = cd->speed / 2;
		cd->bIsleft = true;
	}
	else
	{
		cd->rotateSpeed = -cd->speed / 2;
		cd->bIsleft = false;
	}


	m_list.pushBack(cd);
	addChild(cd);

}


