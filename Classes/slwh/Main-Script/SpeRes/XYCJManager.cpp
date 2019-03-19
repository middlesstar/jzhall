#include "XYCJManager.h"
#ifndef _TRANSPARENT
#define _TRANSPARENT 255
#endif // !_TRANSPARENT

#define FADE_TIME		0.5


//m_node 节点下的逻辑标签
#define XYCJ_TITLE			"xycj_title"
#define TITLE_LEFT			"xycj_res_lef"
#define TITLE_MID			"xycj_res_mid"
#define TITLE_RIGHT			"xycj_res_right"
#define ZP_BACK				"back"

//ZP_BACK节点下的转盘单元数组
#define ZP_ARRAY			{"index", "index_0","index_1","index_2","index_3","index_4","index_5","index_6","index_7","index_8","index_9", "index_10", "index_11", "index_12", "index_13", "index_14"}

//幸运抽奖

//转盘普通
#define	Path_XYCJ_Normal_Root				"game/game_results/gr_prize_%d.png"
//转盘金色
#define	Path_XYCJ_Gold_Root					"game/game_results/gr_prize_1%02d.png"
//元宝图片
#define	Path_XYCJ_Normal_YB					"game/game_results/gr_prize_yb.png"
#define	Path_XYCJ_Gold_YB					"game/game_results/gr_prize_goldyb.png"
#define	Path_XYCJ_Normal_Ticket				"game/game_results/gr_prize_cp.png"
#define	Path_XYCJ_Gold_Ticket				"game/game_results/gr_prize_goldcp.png"


//转盘转动属性
#define	ADD_SPEED		10
#define MAX_SPEED		50              //最大速度    
#define	SUB_SPEED		20
#define INIT_SPEED		0.41f

//0 8 为A区间取服务端的值 5 13 为元宝乘以0.01f


float XYCJManager::m_beilv[16] = { 0, 0, 50, 5, 20, 0.5, 40, 15, 0, 5, 80, 30, 10, 0.1, 25, 10 };	//XYCJ转盘倍率
XYCJManager::XYCJManager():
m_resMid(nullptr),
m_resLeft(nullptr),
m_resRight(nullptr),
m_title(nullptr),
m_pListen(nullptr),
m_ps(nullptr),
m_index(_ZERO),
m_resNowIndex(_ZERO),
m_speed(INIT_SPEED),
m_scoreBl(_ZERO),
m_yuanbaobl(_ZERO),
m_cpBl(_ZERO)
{
	memset(m_list, NULL, sizeof(m_list));
	memset(m_goldlist, NULL, sizeof(m_goldlist));
}

XYCJManager::~XYCJManager()
{
}

void XYCJManager::StartPlayEff()
{
	//创建特效
	m_ps = PUParticleSystem3D::create("slwh/ps3d/scripts/example_010.pu");
	CC_ASSERT(m_ps != nullptr);
	m_ps->setPosition3D(Vec3(_ZERO, 50, _ZERO));
	m_ps->setScale(10.f);
	m_ps->setCameraMask((int)CameraFlag::USER1);
	m_ps->startParticleSystem();
	addChild(m_ps, 0);
}

void XYCJManager::StopPlayEff()
{
	if (m_ps != nullptr)
	{
		m_ps->removeFromParent();
		m_ps = nullptr;
	}
}

void XYCJManager::Start()
{

	//颜色层
	Vec2 visibleSize = Director::getInstance()->getWinSize();
	Color4B color = Color4B(_ZERO, _ZERO, _ZERO, COLOR_LAYER_TANSPARENT);
	CCLayerColor * layer = CCLayerColor::create(color);
	layer->setContentSize(CCSizeMake(visibleSize.x, visibleSize.y));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(visibleSize / 2);
	m_node->addChild(layer, COLOR_LAYER_LEVEL);


	m_resMid = dynamic_cast<ImageView*>(m_node->getChildByName(TITLE_MID));
	CC_ASSERT(nullptr != m_resMid);

	m_resLeft = dynamic_cast<ImageView*>(m_node->getChildByName(TITLE_LEFT));
	CC_ASSERT(nullptr != m_resLeft);

	m_resRight = dynamic_cast<ImageView*>(m_node->getChildByName(TITLE_RIGHT));
	CC_ASSERT(nullptr != m_resRight);

	m_title = dynamic_cast<ImageView*>(m_node->getChildByName(XYCJ_TITLE));
	CC_ASSERT(nullptr != m_title);


	std::string index[] = ZP_ARRAY;
	auto zp = m_node->getChildByName(ZP_BACK)->getChildByName("Normal");
	auto goldzp = m_node->getChildByName(ZP_BACK)->getChildByName("Gold");
	for (size_t i = _ZERO; i < ZP_MAX; i++)
	{
		m_list[i] = dynamic_cast<ImageView*>(zp->getChildByName(index[i]));
		CC_ASSERT(nullptr != m_list[i]);

		m_goldlist[i] = dynamic_cast<ImageView*>(goldzp->getChildByName(index[i]));
		CC_ASSERT(nullptr != m_goldlist[i]);
		m_goldlist[i]->setVisible(false);
	}

	
	//关联路径
	char buf[256];
	for (size_t i = _ZERO; i < ZP_MAX; i++)
	{
		//A区间暂时设置为元宝
		if (i == 0 || i == 5 || i == 8 || i == 13)
		{
			m_normalMap[i] = Path_XYCJ_Normal_YB;
			m_goldMap[i] = Path_XYCJ_Gold_YB;
			continue;
		}

		sprintf(buf, Path_XYCJ_Normal_Root,i);
		m_normalMap[i] = buf;
		sprintf(buf, Path_XYCJ_Gold_Root,i);
		m_goldMap[i] = buf;
	}

	m_node->setOpacity(_ZERO);
	m_node->setVisible(false);

	//测试
	/*int x[] = { 1, 15, 3, 0 };
	StartPlay(1,x);*/
	//StartPlayEff();
}

void XYCJManager::StartPlay(int Atype,float Avalue,int* x)
{
	if (m_node == nullptr) return;

	m_node->setVisible(true);

	m_title->setOpacity(_TRANSPARENT);
	m_resMid->stopAllActions();
	m_resMid->setOpacity(_ZERO);
	m_resLeft->stopAllActions();
	m_resLeft->setOpacity(_ZERO);
	m_resRight->stopAllActions();
	m_resRight->setOpacity(_ZERO);

	for (size_t i = _ZERO; i < RES_MAX; i++)
	{
		if (i == _ZERO)
		{
			m_resBufRun[_ZERO] = x[_ZERO] + 1;
		}
		else
		{
			m_resBufRun[i] = x[i] - x[i - 1];
		}
			
	}
								

	memcpy(m_res,x,sizeof(m_res));

	m_resNowIndex = _ZERO;
	m_node->runAction(FadeIn::create(FADE_TIME));
	scheduleOnce(schedule_selector(XYCJManager::Running), FADE_TIME + m_speed);

	//设置A区间 位置0 和位置8
	m_beilv[0] = m_beilv[8] = Avalue;
	if (Atype == 1)
	{
		//元宝
		m_normalMap[0] = m_normalMap[8] = Path_XYCJ_Normal_YB;
		m_goldMap[0] = m_goldMap[8] = Path_XYCJ_Gold_YB;	
	}
	else
	{
		//船票
		m_normalMap[0] = m_normalMap[8] = Path_XYCJ_Normal_Ticket;
		m_goldMap[0] = m_goldMap[8] = Path_XYCJ_Gold_Ticket;
	}

	//A区间刷新图片
	m_list[0]->loadTexture(m_normalMap[0], TextureResType::PLIST);
	m_list[8]->loadTexture(m_normalMap[8], TextureResType::PLIST);

	m_goldlist[0]->loadTexture(m_goldMap[0], TextureResType::PLIST);
	m_goldlist[8]->loadTexture(m_goldMap[8], TextureResType::PLIST);

	m_scoreBl = _ZERO;
	m_yuanbaobl = _ZERO;
	m_cpBl = _ZERO;

	for (size_t i = _ZERO; i < 4; i++)
	{
		//中了A区间
		if (x[i] == 0 || x[i] == 8)
		{
			if (Atype == 1)
			{
				//中的是元宝
				m_yuanbaobl += Avalue;
			}
			else
			{
				//中的是船票
				m_cpBl += Avalue;
			}
		}
		//中了副对角线的元宝
		else if (x[i] == 5 || x[i] == 13)
		{
			m_yuanbaobl += m_beilv[x[i]];
		}
		//中了金币
		else
		{
			m_scoreBl += (int)m_beilv[x[i]];
		}

		//如果不是luck 就结束算分
		if (x[0] != 1) break;

	}


	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(XYCJManager::onTouchBegan, this);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool XYCJManager::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}



void XYCJManager::StopPlay()
{


	m_node->runAction(CCSequence::create(
		FadeOut::create(FADE_TIME),
		CCCallFunc::create(this, callfunc_selector(XYCJManager::CallBack)),
		nullptr
		)); 

}

//完全消失后还原
void XYCJManager::CallBack()
{
	//还原终点     
	if (m_res[_ZERO] != 1)
	{
		m_goldlist[m_index]->setVisible(false);
		m_resMid->stopAllActions();
	}
	else
	{

		for (size_t i = _ZERO; i < 4; i++)
		{
			m_goldlist[m_res[i]]->setVisible(false);
		}
		//虽然这里有停止动画 但是有时候还是会出问题：第二次进入XYCJ的时候 这3个图标就在那里闪动 在StartPlay方法下面增加了停止动画的调用 
		m_resLeft->stopAllActions();
		m_resMid->stopAllActions();
		m_resRight->stopAllActions();
	}
	m_index = _ZERO;
	m_node->setVisible(false);

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->removeEventListenersForTarget(this);
}

//延迟开启过渡
void XYCJManager::Running(float t)
{
	scheduleUpdate();
}

void XYCJManager::update(float t)
{
	static float time = _ZERO;
	static int index = _ZERO;
	if (time >= m_speed)
	{
		time = _ZERO;

		//加速
		if (index < ADD_SPEED)
		{
			oneStep();
			m_speed -= 0.04f;
			index++;
		}
		//最大速度匀速
		else if (index <  ADD_SPEED + MAX_SPEED + m_resBufRun[m_resNowIndex])
		{
			oneStep();
			index++;
		}
		//减速运动
		else if (index < ADD_SPEED + MAX_SPEED + m_resBufRun[m_resNowIndex] + SUB_SPEED)
		{
			if (index > ADD_SPEED + MAX_SPEED + m_resBufRun[m_resNowIndex] + SUB_SPEED - 6)
			{
				_PLAY_MUSCI_EFF(XYCJ_RUN);
			}

			oneStep();
			m_speed += 0.02f;
			index++;
		}

		else
		{
			index = _ZERO;
			unscheduleUpdate();
			scheduleOnce(schedule_selector(XYCJManager::Res), 0.5f);
		}
		
	}
	time += t;
}

void XYCJManager::Res(float t)
{
	_PLAY_MUSCI_EFF(XYCJ_STOP);
	if (m_res[_ZERO] == 1)
	{
		if (++m_resNowIndex < RES_MAX)
		{
			m_speed = INIT_SPEED;
			scheduleOnce(schedule_selector(XYCJManager::Running), m_speed);
		}
		else
		{


			m_resLeft->loadTexture(m_goldMap[m_res[1]], TextureResType::PLIST);
			m_resMid->loadTexture(m_goldMap[m_res[2]], TextureResType::PLIST);
			m_resRight->loadTexture(m_goldMap[m_res[3]], TextureResType::PLIST);

			m_title->runAction(FadeOut::create(FADE_TIME));

			m_resLeft->runAction(RepeatForever::create(
				Sequence::create(
				FadeIn::create(FADE_TIME),
				FadeOut::create(FADE_TIME),
				nullptr)));

			m_resMid->runAction(RepeatForever::create(
				Sequence::create(
				FadeIn::create(FADE_TIME),
				FadeOut::create(FADE_TIME),
				nullptr)));

			m_resRight->runAction(RepeatForever::create(
				Sequence::create(
				FadeIn::create(FADE_TIME),
				FadeOut::create(FADE_TIME),
				nullptr)));
		}

	}
	else
	{
		//索引多了1  
		if (--m_index < _ZERO)
			m_index = ZP_MAX - 1;

		m_resMid->loadTexture(m_goldMap[m_index], TextureResType::PLIST);
		m_title->runAction(FadeOut::create(FADE_TIME));
		m_resMid->runAction(RepeatForever::create(
			Sequence::create(
			FadeIn::create(FADE_TIME),
			FadeOut::create(FADE_TIME),
			nullptr)));
	}
}

void XYCJManager::oneStep()
{

	//Music.GetComponent<MusicManager>().Play(enumMusic.run);

	int index = m_index;

	//改变当前颜色
	m_goldlist[index]->setVisible(true);

	//出现一条尾巴
	if (m_speed < 0.2f)
	{
		if (--index < _ZERO)
			index = ZP_MAX - 1;

		m_goldlist[index]->setVisible(true);
		m_goldlist[index]->setOpacity(_TRANSPARENT * 0.8f);
	}

	//出现2条尾巴
	if (m_speed < 0.15f)
	{
		if (--index < _ZERO)
			index = ZP_MAX - 1;

		m_goldlist[index]->setVisible(true);
		m_goldlist[index]->setOpacity(_TRANSPARENT * 0.5f);
	}


	//取消掉最前面的尾巴
	if (--index < _ZERO)
		index = ZP_MAX - 1;
	m_goldlist[index]->setVisible(false);
	m_goldlist[index]->setOpacity(_TRANSPARENT);
	

	if (--index < _ZERO)
		index = ZP_MAX - 1;
	m_goldlist[index]->setVisible(false);
	m_goldlist[index]->setOpacity(_TRANSPARENT);


	if (++m_index > ZP_MAX - 1)
		m_index = _ZERO;

	if (m_res[_ZERO] == 1 && m_resNowIndex >= 1)
	{
		for (int i = _ZERO; i < m_resNowIndex; i++)
		{	
			m_goldlist[m_res[i]]->setVisible(true);
			m_goldlist[m_res[i]]->setOpacity(_TRANSPARENT);
		}
	}
}

