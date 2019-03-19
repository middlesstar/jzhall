#include "CD_SHZ.h"

#ifndef _ZERO
#define _ZERO	0
#endif


#ifndef		PLIST_PATH
#define		PLIST_PATH							"mainscene/shz_game.plist"
#endif // !PLIST_PATH




#define		CD_OFFSET							8


//彩带运动属性
#define		MIN_SPEED							40
#define		MAX_SPEED							70
#define		LR_SPEED_MIN						10
#define		LR_SPEED_MAX						20


CD* CD::m_instance = NULL;
CD::CD() :
m_bIsOpen(false),
m_type(_ZERO)
{
}


CD::~CD()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("mainscene/shz_game.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("mainscene/shz_game.png");
}



CD* CD::GetInstance()
{
	if (NULL == m_instance)
	{
		m_instance = new CD();
		m_instance->init();
	}

	return m_instance;
}


void CD::StartPlay(unsigned int n)
{
	m_type = n;
	m_bIsOpen = true;
	m_list.clear();
	removeAllChildren();
	scheduleUpdate();
}


void CD::StopPlay()
{
	m_bIsOpen = false;
}


void CD::update(float deltaTime)
{
	static int m = _ZERO;
	if (m_bIsOpen)
	{
		if (++m > 20)
		{
			m = _ZERO;
			CreatOne();
		}
	}


	for (Vector<OneCD*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		OneCD* cd = (*itor);
		Vec2 v2 = cd->getPosition();
		if (cd->bIsleft)
		{
			v2 += Vec2(-random(LR_SPEED_MIN, LR_SPEED_MAX) * deltaTime*random(3,6), -cd->speed * deltaTime);
		}
		else
		{
			v2 += Vec2(random(LR_SPEED_MIN, LR_SPEED_MAX) * deltaTime* deltaTime*random(3, 6), -cd->speed * deltaTime);
		}

		cd->setPosition(v2);

		if (random(_ZERO, 80) == _ZERO)
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



bool CD::init()
{

	m_visibleSize = Director::getInstance()->getWinSize();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH);
	for (size_t i = _ZERO; i < CD_MAX; i++)
	{
		std::stringstream str;
		str << Path_PNG_LEAVES_Root << i << ".png";
		m_texture2D_leaves[i] = str.str();


	}

	for (size_t i = _ZERO; i < CD_MAX; i++)
	{
		std::stringstream str;
		str << Path_PNG_FIRE_Root << i << ".png";
		m_texture2D_fire[i] = str.str();
	}


	setName("CDFallen");
	scheduleUpdate();

	return true;
}


void CD::CreatOne()
{
	int x = random(_ZERO, CD_MAX - 1);
	OneCD *cd = nullptr;
	switch (m_type)
	{
	case 0:
		cd = OneCD::create(m_texture2D_leaves[x]);
		break;
	case 1:
		cd = OneCD::create(m_texture2D_fire[x]);
		break;
	default:
		CC_ASSERT(true);
		break;
	}


	//起始位置
	cd->setPosition(Vec2(random(0.f, m_visibleSize.x), m_visibleSize.y + CD_OFFSET));

	//缩放
	cd->setScale(random(0.5f, 1.0f));
	//下落速度
	cd->speed = random(MIN_SPEED, MAX_SPEED);
	if (random(0, 1) == 0)
	{
		cd->rotateSpeed = cd->speed ;
		cd->bIsleft = true;
	}
	else
	{
		cd->rotateSpeed = -cd->speed ;
		cd->bIsleft = false;
	}


	m_list.pushBack(cd);
	addChild(cd);

}
