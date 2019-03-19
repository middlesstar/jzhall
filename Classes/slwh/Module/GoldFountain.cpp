#include "GoldFountain.h"


//金币运动轨迹为抛物线 通过随机 让金币初速度不同开呈现开花的效果，通过运动时间的不同让金币停留在散乱的范围内


#ifndef _ZERO
#define _ZERO	0
#endif

GoldFountain::GoldFountain() :
now_index(_ZERO),
m_StartPos(_ZERO, _ZERO),
m_EndPos(_ZERO, _ZERO),
m_bIsOpen(false),
Gold_V_X_MIN(_ZERO),
Gold_V_X_MAX(_ZERO),
Gold_V_Y_MIN(_ZERO),
Gold_V_Y_MAX(_ZERO),
Gold_V_TIME_MIN(_ZERO),
Gold_V_TIME_MAX(_ZERO),
Gold_V_TIME_PAUSE(_ZERO),
Gold_V_TIME_SHOOT(_ZERO),
Gold_Rotate_Speed(_ZERO),
Gold_G(_ZERO),
//Max_Gold(_ZERO),	
Max_Gold_Time(_ZERO),
Layer_Level(_ZERO)					
{
}

bool GoldFountain::init()
{
	if (false == Node::init())
	{
		return false;
	}
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH);
	return true;
}

GoldFountain::~GoldFountain()
{
}

void GoldFountain::update(float deltaTime)
{

	//每帧生产一个金币
	if (m_bIsOpen)
	{
		now_index += deltaTime;
		if (now_index > Max_Gold_Time)
		{
			m_bIsOpen = false;
		}
		CreatOne();
	}

	//位置临时变量
	Vec2 v2;
	for (Vector<OneGold*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		OneGold *index = (OneGold*)(*itor);
		index->time -= deltaTime;
		if (index->time >= Gold_V_TIME_PAUSE + Gold_V_TIME_SHOOT)
		{
			index->v_y -= Gold_G * deltaTime;
			v2 = index->getPosition();
			v2 += Vec2(index->v_x *deltaTime, index->v_y *deltaTime);
			index->setPosition(v2);
		}
		else if (index->time >= Gold_V_TIME_SHOOT)
		{
			//金币暂停 不位移 打个酱油
		}
		else if (index->time >= _ZERO)
		{
			//金币开始冲刺到终点
			if (index->bIsShoot == false)
			{
				index->bIsShoot = true;
				index->runAction(CCMoveTo::create(Gold_V_TIME_SHOOT, m_EndPos));
				index->runAction(FadeOut::create(Gold_V_TIME_SHOOT));
			}
		}

		if (index->bIsShoot && index->getPosition() == m_EndPos)
		{
			removeChild(index, true);
			itor = m_list.erase(itor);	
		}
		else
		{
			itor++;
		}
	}
}


//创建一枚金币
void GoldFountain::CreatOne()
{
	OneGold *g = OneGold::create(Path_PNG_Gold);
	//缩放0.8我觉得比较好看
	g->setScale(0.8);
	//起始位置
	g->setPosition(Vec2(_ZERO,_ZERO));
	//运动时间
	g->time = random(Gold_V_TIME_MIN + Gold_V_TIME_PAUSE + Gold_V_TIME_SHOOT, Gold_V_TIME_MAX + Gold_V_TIME_PAUSE + Gold_V_TIME_SHOOT);
	//设置状态
	g->bIsShoot = false;
	//随机初速度
	g->v_x = random(Gold_V_X_MIN, Gold_V_X_MAX);
	g->v_y = random(Gold_V_Y_MIN, Gold_V_Y_MAX);
	

	g->runAction(RepeatForever::create(RotateBy::create(Gold_Rotate_Speed, Vec3(0, 360, 0))));
	m_list.pushBack(g);
	addChild(g);
}


//开启动画
GoldFountain* GoldFountain::StartPlay(Vec2 StartPos, Vec2 EndPos,int type)
{
	Scene* main = Director::getInstance()->getRunningScene();
	GoldFountain* This = new GoldFountain();
	
	if (This != nullptr && This->init())
	{
		switch (type)
		{
		case 0:
			This->Gold_V_X_MIN = -100.f; 				//横向初速度最小值
			This->Gold_V_X_MAX = 100.f;					//横向初速度最大值
			This->Gold_V_Y_MIN = 300.f;					//纵向初速度最小值
			This->Gold_V_Y_MAX = 500.f;					//纵向初速度最大值
			This->Gold_V_TIME_MIN = 0.5f;				//金币运动最小时间
			This->Gold_V_TIME_MAX = 1.f;				//金币运动最大时间
			This->Gold_V_TIME_PAUSE = 0.5f;				//金币停留时间
			This->Gold_V_TIME_SHOOT = 0.5f;				//金币最后冲刺时间
			This->Gold_Rotate_Speed = 0.5f;				//硬币自转一圈的时间
			This->Gold_G = 700;							//重力加速度(只对抛物运动)
			This->Max_Gold_Time = 3.f;						//金币产生的个数
			This->Layer_Level = 200;					//层级
			break;
		case 1:
			This->Gold_V_X_MIN = -300.f; 				//横向初速度最小值
			This->Gold_V_X_MAX = 300.f;					//横向初速度最大值
			This->Gold_V_Y_MIN = 500.f;					//纵向初速度最小值
			This->Gold_V_Y_MAX = 800.f;					//纵向初速度最大值
			This->Gold_V_TIME_MIN = 0.5f;				//金币运动最小时间
			This->Gold_V_TIME_MAX = 1.f;				//金币运动最大时间
			This->Gold_V_TIME_PAUSE = 0.5f;				//金币停留时间
			This->Gold_V_TIME_SHOOT = 0.5f;				//金币最后冲刺时间
			This->Gold_Rotate_Speed = 0.5f;				//硬币自转一圈的时间
			This->Gold_G = 1000;						//重力加速度(只对抛物运动)
			This->Max_Gold_Time = 3.f;						//金币产生的个数
			This->Layer_Level = 200;					//层级
			break;
		default:
			break;
		}
	

		This->setPosition(StartPos);
		This->m_StartPos = StartPos;
		This->m_EndPos = EndPos - StartPos;
		This->m_bIsOpen = true;
		This->autorelease();
		main->addChild(This, This->Layer_Level);
		This->scheduleUpdate();
		This->scheduleOnce(schedule_selector(GoldFountain::Destory), 10);
	}
	return This;
}


void GoldFountain::Destory(float t)
{
	this->removeFromParent();
}

