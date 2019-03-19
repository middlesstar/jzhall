#include "GoldFallen3D.h"
#include "slwh/ResourcePath.h"



#define		Gold_Jump_Min	0.5f
#define		Gold_Jump_Max	0.8f
#define		Gold_Offset		50
#define		Gold_G			1500

#ifndef _ZERO
#define _ZERO	0
#endif

//静态变量初始化
GoldFallen3D* GoldFallen3D::m_instance = NULL;
GoldFallen3D::GoldFallen3D() :
SpeedAdd(Gold_G),
now_index(_ZERO),
m_bIsOpen(false)
{
}

bool GoldFallen3D::init()
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
		str << GoldFallen3D_GOLD << i << ".png";
		m_Texture[i] = str.str();
	}
	setName("GoldFallen3D");
	scheduleUpdate();
	return true;
}

GoldFallen3D::~GoldFallen3D()
{
}


GoldFallen3D* GoldFallen3D::GetInstance()
{

	if (NULL == m_instance)
	{
		m_instance = new GoldFallen3D();
		m_instance->init();
	}

	return m_instance;
}

void GoldFallen3D::update(float deltaTime)
{

	//每帧生产一个金币
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
	Vec3 v3;

	for (Vector<OneGold*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		OneGold *index = (OneGold*)(*itor);

		index->v_y += (float)SpeedAdd * deltaTime;

		v3 = index->getPosition3D();
		v3 -= Vec3(index->v_x * deltaTime, index->v_y *deltaTime, deltaTime * index->v_z);
		
		index->setPosition3D(v3);

		index->time += deltaTime;

		if (index->time >= 0.1f)
		{
			index->time = _ZERO;
			index->setTexture(m_Texture[random(_ZERO, Max_Tex - 1)]);
			//index->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_Texture[random(_ZERO, Max_Tex - 1)]));
		}

		if (v3.y < -Gold_Offset )
		{
			if (false == index->jump)
			{
				v3.y = _ZERO;
				index->setPosition3D(v3);
				index->v_y = -index->v_y * random(Gold_Jump_Min, Gold_Jump_Max);
				index->jump = true;
				index->v_z = random(-1000, -500);
				index->v_x = index->getPosition().x - m_visibleSize.x / 2;						//打脸飞
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
void GoldFallen3D::CreatOne()
{

	OneGold *g = OneGold::create(m_Texture[random(_ZERO, Max_Tex - 1)]);
	//缩放0.8我觉得比较好看
	g->setScale(0.8);
	//起始位置
	g->setPosition(random(0.f, m_visibleSize.x), m_visibleSize.y + Gold_Offset);
	//下落速度
	g->time = _ZERO;
	g->v_x = _ZERO;
	g->v_y = _ZERO;				
	g->v_z = _ZERO;
	g->jump = false;
	m_list.pushBack(g);
	addChild(g);
}


//开启动画
void GoldFallen3D::StartPlay()
{
	Scene* main = Director::getInstance()->getRunningScene();
	GoldFallen3D* This = GetInstance();
	if (!main->getChildByName("GoldFallen3D"))
	{
		main->addChild(This, 200);
	}
	This->m_bIsOpen = true;
	This->scheduleUpdate();
}


//停止生产金币
void GoldFallen3D::StopPlay()
{
	GetInstance()->now_index = 0;
	GetInstance()->m_bIsOpen = false;
}


