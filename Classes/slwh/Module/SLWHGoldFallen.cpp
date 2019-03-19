#include "SLWHGoldFallen.h"
#include "slwh/ResourcePath.h"



#define		Gold_Jump_Min	0.5f
#define		Gold_Jump_Max	0.8f
#define		Gold_Offset		50
#define		Gold_G			1500

#ifndef _ZERO
#define _ZERO	0
#endif

//静态变量初始化
SLWH_GoldFallen* SLWH_GoldFallen::m_instance = NULL;
SLWH_GoldFallen::SLWH_GoldFallen() :
SpeedAdd(Gold_G),
now_index(_ZERO),
m_bIsOpen(false)
{
}

bool SLWH_GoldFallen::init()
{
	if (false == Node::init())
	{
		return false;
	}
	m_visibleSize = Director::getInstance()->getWinSize();
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH);

	for (size_t i = _ZERO; i < Max_Tex; i++)
	{
		std::stringstream str;
		str << SLWH_GoldFallen_Gold << i << ".png";
		m_Texture[i] = str.str();
	}
	setName("GoldFallen");
	scheduleUpdate();
	return true;
}

SLWH_GoldFallen::~SLWH_GoldFallen()
{
}


SLWH_GoldFallen* SLWH_GoldFallen::GetInstance()
{

	if (NULL == m_instance)
	{
		m_instance = new SLWH_GoldFallen();
		m_instance->init();
	}

	return m_instance;
}

void SLWH_GoldFallen::update(float deltaTime)
{

	//每帧产生一个金币
	if (m_bIsOpen)
	{
		/*	time += deltaTime;
		if (time > 0.01f)
		{*/
		if (++now_index > Max_Gold)
		{
			StopPlay();
		}
		CreatOne();
		//	time = 0;
		//}
	}

	//位置临时变量
	Vec2 v2;

	for (Vector<SLWH_OneGold*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		SLWH_OneGold *index = (SLWH_OneGold*)(*itor);

		index->v_y += (float)SpeedAdd * deltaTime;

		v2 = index->getPosition();
		v2 -= Vec2(_ZERO, index->v_y *deltaTime);

		index->setPosition(v2);

		index->time += deltaTime;

		if (index->time >= 0.1f)
		{
			index->time = _ZERO;
			index->setTexture(m_Texture[random(_ZERO, Max_Tex - 1)]);
			//index->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_Texture[random(_ZERO, Max_Tex - 1)]));
		}

		if (v2.y < -Gold_Offset)
		{
			if (false == index->jump)
			{
				v2.y = _ZERO;
				index->setPosition(v2);
				index->v_y = -index->v_y * random(Gold_Jump_Min, Gold_Jump_Max);
				index->jump = true;
				++itor;
			}
			else
			{
				removeChild(index, true);
				itor = m_list.erase(itor);
			}

		}
		else
		{
			++itor;
		}
	}
}


//创建一枚金币
void SLWH_GoldFallen::CreatOne()
{
	SLWH_OneGold *g = SLWH_OneGold::create(m_Texture[random(_ZERO, Max_Tex - 1)]);
	//缩放0.8我觉得比较好看
	g->setScale(0.8);
	//起始位置
	g->setPosition(random(0.f, m_visibleSize.x), m_visibleSize.y + Gold_Offset);
	//下落速度
	g->time = _ZERO;
	g->v_y = _ZERO;
	g->jump = false;
	m_list.pushBack(g);
	addChild(g);
}


//开启动画
void SLWH_GoldFallen::StartPlay()
{
	Scene* main = Director::getInstance()->getRunningScene();
	SLWH_GoldFallen* This = GetInstance();
	if (!main->getChildByName("GoldFallen"))
	{
		main->addChild(This, 200);
	}
	This->m_bIsOpen = true;
	This->scheduleUpdate();
}


//停止生产金币
void SLWH_GoldFallen::StopPlay()
{
	GetInstance()->now_index = 0;
	GetInstance()->m_bIsOpen = false;
}


