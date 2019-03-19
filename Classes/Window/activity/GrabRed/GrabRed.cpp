#include "GrabRed.h"
#include "ActiveConfig.h"
/*
防止作弊采取内存数据加密
加上一个常数
*/
int _RAND0 = _ZERO;
int _RAND1 = _ZERO;
int _RAND2 = _ZERO;

Vec2 VISIBLE_SIZE;

//***************************************************************************************
//抢红包管理类.cpp

GrabRed::GrabRed() :
m_backImg(NULL),
m_topImg(NULL),
m_wace0(NULL),
m_wace1(NULL),
m_text(NULL),
m_bIsActivity(false)
{
}

void GrabRed::SetCallback(Node* n, SEL_CallFun callback)
{
	m_pListen = n;
	m_pfnSelectior = callback;
}

GrabRed::~GrabRed()
{
	//SimpleAudioEngine::getInstance()->end();		//释放音乐引擎变量所占资源
}

bool GrabRed::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//加载音乐和音效文件
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_BK);
	SimpleAudioEngine::getInstance()->preloadEffect(MUSIC_FISH_JUMP);
	SimpleAudioEngine::getInstance()->preloadEffect(MUSCI_EAT_PACKET);
	SimpleAudioEngine::getInstance()->preloadEffect(MUSCI_EAT_BOMB_GET);
	SimpleAudioEngine::getInstance()->preloadEffect(MUSCI_EAT_BOMB_DIE);


	//设置默认音量
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);



	_RAND0 = random(-0x0000FFFF, 0x0000FFFF);
	_RAND1 = random(-0x0000FFFF, 0x0000FFFF);
	_RAND2 = random(-0x0000FFFF, 0x0000FFFF);

	VISIBLE_SIZE = Director::getInstance()->getWinSize();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH);

	
	m_backImg = Sprite::createWithSpriteFrameName(_BK);
	CC_ASSERT(NULL != m_backImg);
	m_backImg->setPosition(Vec2(VISIBLE_SIZE.x / 2, VISIBLE_SIZE.y / 2));
	addChild(m_backImg);


	m_topImg = Sprite::createWithSpriteFrameName(_TOP);
	CC_ASSERT(NULL != m_topImg);
	m_topImg->setAnchorPoint(Vec2(0.5f, 1.f));
	m_topImg->setPosition(Vec2(VISIBLE_SIZE.x / 2, VISIBLE_SIZE.y));
	addChild(m_topImg);

	m_wace0 = Sprite::createWithSpriteFrameName(_WAVE0);
	CC_ASSERT(NULL != m_wace0);
	m_wace0->setAnchorPoint(Vec2(0.5f, 0.f));
	m_wace0->setPosition(Vec2(VISIBLE_SIZE.x / 2, 30));
	addChild(m_wace0);

	//进行字符串转换
	m_text = Text::create(TEXT_CONTENT, TEXT_FONT, TEXT_SIZE);
	m_text->setAnchorPoint(Vec2(1.f, 1.f));
	m_text->setPosition(Vec2(VISIBLE_SIZE.x -10, VISIBLE_SIZE.y - 25));					
	addChild(m_text);



	m_info.OnInit();
	m_info.setPosition(Vec2(_ZERO, _ZERO));
	addChild(&m_info);


	m_fish.OnInit();
	m_fish.setPosition(Vec2(_ZERO, _ZERO));
	addChild(&m_fish);


	m_wace1 = Sprite::createWithSpriteFrameName(_WAVE1);
	CC_ASSERT(NULL != m_wace1);
	m_wace1->setAnchorPoint(Vec2(0.5f, 0.f));
	m_wace1->setPosition(Vec2(VISIBLE_SIZE.x / 2, _ZERO));
	addChild(m_wace1);


	m_redPacket.OnInit();
	m_redPacket.setPosition(Vec2(_ZERO, _ZERO));
	addChild(&m_redPacket);



	//允许UI层接收触摸事件  
	this->setTouchEnabled(true);
	

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GrabRed::onTouchBegan, this);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//播放背景
	_PLAY_MUSCI_BACK(MUSIC_BK);

	////测试代码
	//auto bt = Button::create();
	//bt->setPosition(Vec2(VISIBLE_SIZE.x / 2, VISIBLE_SIZE.y -40));
	//bt->setTitleText("Start again");
	////bt->setVisible(false);
	//bt->addTouchEventListener(this,toucheventselector(GrabRed::BtEvent));
	//bt->setScale(2);
	//addChild(bt);

	return true;

}

bool GrabRed::onTouchBegan(Touch* touch, Event* event)
{
	m_fish.Jump();
	return true;
}

////测试
//void GrabRed::BtEvent(Ref* pSender, TouchEventType type)
//{
//	switch (type)
//	{
//	case TOUCH_EVENT_BEGAN:
//		((Node*)pSender)->setColor(Color3B(180, 180, 180));
//		break;
//	case TOUCH_EVENT_MOVED:
//
//		break;
//	case TOUCH_EVENT_ENDED:
//		((Node*)pSender)->setColor(Color3B(255, 255, 255));
//		
//		m_info.InitData();
//		StartPlay();
//		break;
//	case TOUCH_EVENT_CANCELED:
//		((Node*)pSender)->setColor(Color3B(255, 255, 255));
//		break;
//	default:
//		break;
//	}
//}

//得到游戏走过的时间
float GrabRed::GetTime()
{
	return m_info.GetTime();
}


//得到话费
float GrabRed::GetHF()
{
	return m_info.GetHF();
}


//得到金币
int GrabRed::GetGold()
{
	return m_info.GetGold();
}

//碰撞检测
void GrabRed::update(float t)
{
	Vector<RedPacket::OnePacket*>* list = m_redPacket.GetList();

	for (Vector<RedPacket::OnePacket*>::iterator itor = list->begin(); itor != list->end(); /*++itor*/)
	{
		RedPacket::OnePacket *index = (RedPacket::OnePacket*)(*itor);
		if (m_fish.GetRect().intersectsRect(index->boundingBox()))
		{
			int gold = _ZERO;
			float hf = _ZERO;
			if (index->m_type == RedPacket::OnePacket::BOMB_GET)
			{
				m_redPacket.BombGet(gold, hf);
				m_info.AddGold(gold);
				m_info.AddHF(hf);

				_PLAY_MUSCI_EFF(MUSCI_EAT_BOMB_GET);
				//容器被清空 不能继续
				break;
			}
			else if (index->m_type == RedPacket::OnePacket::BOMB_DIE)
			{

				_PLAY_MUSCI_EFF(MUSCI_EAT_BOMB_GET);

				m_redPacket.BombDie();

				break;
			}
			else if (
				index->m_type == RedPacket::OnePacket::PACKET_GOLD_SMALL ||
				index->m_type == RedPacket::OnePacket::PACKET_GOLD_MID ||
				index->m_type == RedPacket::OnePacket::PACKET_GOLD_BIG ||
				index->m_type == RedPacket::OnePacket::BAO)
			{
				_PLAY_MUSCI_EFF(MUSCI_EAT_PACKET);


				m_info.AddGold(index->m_num);
			}
			else if (
				index->m_type == RedPacket::OnePacket::PACKET_HF_SMALL ||
				index->m_type == RedPacket::OnePacket::PACKET_HF_BIG)
			{

				_PLAY_MUSCI_EFF(MUSCI_EAT_PACKET);

				m_info.AddHF(index->m_num);

				if (m_info.GetHF() >= PACKET_HF_MAX)
				{
					m_redPacket.SetCreatHF(false);
				}
			}
			m_redPacket.EatPacket(index->m_num, index->m_type, index->getPosition());
			index->removeFromParent();
			itor = list->erase(itor);
		}
		else
		{
			++itor;
		}

	}
	if (m_info.GetTime() >= INFO_TIME_MAX)
	{
		GameOver();
	}
	else
	{
		m_info.AddTime(t);
	}

}

//开始游戏
void GrabRed::StartPlay()
{
	if (m_bIsActivity)
	{
		return;
	}
	//预加载背景音乐
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_BK);
	//预加载特效音乐
	SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_FISH_JUMP);
	SimpleAudioEngine::sharedEngine()->preloadEffect(MUSCI_EAT_PACKET);
	SimpleAudioEngine::sharedEngine()->preloadEffect(MUSCI_EAT_BOMB_GET);
	SimpleAudioEngine::sharedEngine()->preloadEffect(MUSCI_EAT_BOMB_DIE);

	_PLAY_MUSCI_BACK(MUSIC_BK);

	m_bIsActivity = true;
	m_fish.StartPlay();
	m_redPacket.StartPlay();
	m_info.InitData();
	scheduleUpdate();

}


//中断游戏
void GrabRed::StopPlay()
{
	if (!m_bIsActivity)
	{
		return;
	}
	m_bIsActivity = false;
	m_fish.StopPlay();
	m_redPacket.StopPlay();
	unscheduleUpdate();
}

//游戏结束
void GrabRed::GameOver()
{
	//if (!m_bIsActivity)
	//{
	//	return;
	//}
	StopPlay();
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	(m_pListen->*m_pfnSelectior)(m_info.GetGold(), m_info.GetHF());
	//m_bIsActivity = false;
	//m_fish.StopPlay();
	//m_redPacket.StopPlay();

	removeFromParent();
	//unscheduleUpdate();

}

