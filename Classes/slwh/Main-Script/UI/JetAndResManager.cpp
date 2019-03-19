#include "JetAndResManager.h"
#include "slwh/Main-Script/UI/TitleLabel.h"


#define JET_MANAGER			"Jet_Manager"


#define _TIME_NODE			"Time_Node"
//_TIME_NODE下的节点
#define	BET_BT				"bt"
#define _DOT				"dot"
#define _TimeCount			"timenum"
#define _TITLE				"title"

#define _TITLE_PATH_ROOT	"game/game_scene/state/state_display%d.png"

JetAndResManager::JetAndResManager() :
m_jetimg(nullptr),
m_countjettime(_ZERO),
m_bIsOpen(false),
m_bIsEnble(true),
m_title(nullptr),
m_timeCount(nullptr),
m_dot(nullptr),
m_TimeCountNode(nullptr),
m_dotAc(nullptr),
m_moveNode(nullptr),
m_nowTitle(enumPleaseJet)
{
}

JetAndResManager::~JetAndResManager()
{
	if (m_dotAc != nullptr)
	{
		m_dotAc->release();
		m_dotAc = nullptr;
	}
	m_TimeCountNode = nullptr;
	m_jetimg = nullptr;
	m_moveNode = nullptr;
	m_title = nullptr;
	m_timeCount = nullptr;
	m_dot = nullptr;

}


void JetAndResManager::onEnter()
{
	Node::onEnter();

	//m_moveNode->setVisible(false);
	//m_moveNode->setPosition(Vec2(_ZERO, _ZERO));

	////开始是隐藏状态
	//m_TimeCountNode->setVisible(false);
	//m_TimeCountNode->setPosition(Vec2(330, 680));
	//开始倒计时和点
	schedule(schedule_selector(JetAndResManager::updateDot_Time), 1.f);
}


void JetAndResManager::onExit()
{
	Node::onExit();

}

void JetAndResManager::Start()
{

	m_TimeCountNode = dynamic_cast<Node*>(m_node->getChildByName(_TIME_NODE));
	CC_ASSERT(m_TimeCountNode != nullptr);
	//开始是隐藏状态
	m_TimeCountNode->setVisible(false);
	m_TimeCountNode->setPosition(Vec2(330, 680));

	m_jetimg = dynamic_cast<Button*>(m_TimeCountNode->getChildByName(BET_BT));
	CC_ASSERT(m_jetimg != nullptr);
	m_jetimg->addTouchEventListener(this, toucheventselector(JetAndResManager::BtEvent));

	m_dot = dynamic_cast<Sprite*>(m_TimeCountNode->getChildByName(_DOT));
	CC_ASSERT(m_dot != nullptr);

	m_title = dynamic_cast<ImageView*>(m_TimeCountNode->getChildByName(_TITLE));
	CC_ASSERT(m_title != nullptr);

	m_timeCount = dynamic_cast<TextAtlas*>(m_TimeCountNode->getChildByName(_TimeCount));
	CC_ASSERT(m_timeCount != nullptr);

	//得到移动节点
	m_moveNode = m_node->getChildByName(JET_MANAGER);
	CC_ASSERT(m_moveNode != nullptr);
	m_moveNode->setCascadeColorEnabled(false);
	m_moveNode->setPosition(_ZERO, -DesignResolutionSize.height);


	//创建点的动画
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("slwh/game/game_scene/state/dot.plist");
	Vector<AnimationFrame*> animFrames;
	char dotpath[256];
	for (size_t i = _ZERO; i < 3; i++)
	{
		sprintf(dotpath, "state_dot_%d.png", i + 1);
		CCSpriteFrame* frame = cache->spriteFrameByName(dotpath);

		if (frame)
		{
			CCAnimationFrame* aniFrame = CCAnimationFrame::create(frame, 1, ValueMap());
			animFrames.pushBack(aniFrame);
		}
	}

	CCAnimation *animation = CCAnimation::create(animFrames, 0.5f);
	m_dotAc = RepeatForever::create(CCAnimate::create(animation));
	m_dotAc->retain();
	//m_dot->runAction(RepeatForever::create(m_dotAc));
}

void JetAndResManager::initposition()
{
	if (!m_moveNode) return;
	m_moveNode->setPosition(_ZERO,-DesignResolutionSize.height);
	m_moveNode->setVisible(true);
}

void JetAndResManager::inhideImg()
{
	m_moveNode->setVisible(false);
	m_moveNode->setPosition(Vec2(_ZERO, _ZERO));
}

void JetAndResManager::updateDot_Time(float t)
{
	if (--m_nowTime >= _ZERO)
	{

		setTimeUI(m_nowTime);

		if (!m_bIsOpen)
		{
			setTitle(enumJetScene);
		}
		else
		{
			setTitle(enumPleaseJet);
		}
		//最后5秒开始滴滴滴 要不然 滴的个烦死人
		if (m_nowTime <=5 )
		{
			m_nowTime == _ZERO ? _PLAY_MUSCI_EFF(_TIME0) : _PLAY_MUSCI_EFF(_TIME1);
		}
		
	}
	else if (--m_countjettime >= _ZERO)
	{
		//统计分数text
		setTitle(enumCountJet);

		setTimeUI(m_countjettime);

	}
	else
	{
		if (!m_bIsOpen)
		{
			setTitle(enumOpen);
		}
		else
		{
			setTitle(enumGameScene);
		}

		if (!m_dot->isVisible())
		{
			m_timeCount->setVisible(false);
			m_dot->setVisible(true);
			m_dot->runAction(m_dotAc);
		}	
	}

}

void JetAndResManager::setTitle(TimeCountTitle e)
{
	if (e == m_nowTitle)
		return;

	m_nowTitle = e;
	char buf[256];
	sprintf(buf, _TITLE_PATH_ROOT, (int)e + 1);
	m_title->loadTexture(buf, TextureResType::PLIST);
}

void JetAndResManager::moveTimeCountTojet(bool b)
{
	static bool bo = false;
	if (bo == b) return;
	bo = b;

	//从原节点摘下来
	m_TimeCountNode->retain();
	m_TimeCountNode->removeFromParent();

	if (b)
	{
		//移动时间Node到压分界面
		m_moveNode->addChild(m_TimeCountNode, 500);
		m_TimeCountNode->setPosition(Vec2(680, 700));
		//如果是省略号 继续播放动画
		if (m_nowTime < _ZERO && m_countjettime<_ZERO) m_dot->runAction(m_dotAc);
	}
	else
	{
		//移动时间Node到主游戏界面
		m_node->addChild(m_TimeCountNode, -1);
		m_TimeCountNode->setPosition(Vec2(330, 680));
		//如果是省略号 继续播放动画
		if (m_nowTime < _ZERO && m_countjettime<_ZERO) m_dot->runAction(m_dotAc);
	}
	m_TimeCountNode->release();
}

//开启倒计时
void JetAndResManager::StartPlay(int time)
{
	m_timeCount->setVisible(true);
	m_dot->setVisible(false);

	if (time > 3)
	{
		m_nowTime = time - 3;
		m_countjettime = 3;

		setTimeUI(m_nowTime);
	}
	else
	{
		m_countjettime = time;
		setTimeUI(m_countjettime);

	}
}

void JetAndResManager::setTimeUI(int t)
{
	std::stringstream str;
	str << t;
	m_timeCount->setString(str.str());
}

void JetAndResManager::setVisible(bool b)
{
	m_TimeCountNode->setVisible(b);
	m_bIsEnble = b;
}

void JetAndResManager::BtEvent(Ref* pSender, TouchEventType type)
{
	auto bt = dynamic_cast<Node*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		MyJettonOut();
		//_PLAY_MUSCI_EFF(_CLICK);
		break;
	case TOUCH_EVENT_CANCELED:
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	default:
		break;
	}
}

void JetAndResManager::Attach3Nodes(Node* Open, Node* Jet, Node* Userlist)
{
	if (Open == nullptr || Jet == nullptr || Userlist == nullptr)
	{
		//CC_ASSERT(false,"point null JetAndResManager::Attach3Nodes");
		return;
	}
	m_moveNode->addChild(Open);
	m_moveNode->addChild(Jet);
	m_moveNode->addChild(Userlist);
}

bool JetAndResManager::GetIsOpen()
{
	return m_bIsOpen;
}

//*****************************************************************************************

//动画

void JetAndResManager::show(bool b)
{
	if (m_bIsOpen == b) return;
	m_bIsOpen = b;
	if (m_bIsOpen)
	{
		//打开	
		m_moveNode->stopAllActions();
		m_moveNode->runAction(MoveTo::create(0.2f, Vec2(_ZERO, _ZERO)));
		
	}
	else
	{
		//关闭
		m_moveNode->stopAllActions();
		m_moveNode->runAction(MoveTo::create(0.2f, Vec2(_ZERO, -DesignResolutionSize.height)));
	}
}

void JetAndResManager::MyJettonOut(int code /*= 0*/)
{
	if (code == _ZERO)
	{
		//不允许玩家操作
		if (!m_bIsEnble) return;

		if (m_bIsOpen)
		{
			show(false);
		}
		else
		{

			show(true);
		}

	}
	else if (code == 1)
	{
		//强制打开
		show(true);

	}
	else if (code == 2)
	{
		//强制关闭
		show(false);

	}


	if (m_bIsOpen == false)
	{
		if (m_nowTime >= _ZERO)
		{
			setTitle(enumJetScene);
			TitleLabel::SetTitle(TitleLabel::Normal1);
		}
		else if (m_countjettime >= _ZERO)
		{
			setTitle(enumCountJet);
		}
		else
		{
			setTitle(enumOpen);
		}
	}
	else
	{
		if (m_nowTime >= _ZERO)
		{
			setTitle(enumPleaseJet);
		}
		else if (m_countjettime >= _ZERO)
		{
			setTitle(enumCountJet);
		}
		else
		{
			setTitle(enumGameScene);
		}
	}

	moveTimeCountTojet(m_bIsOpen);

	_PLAY_MUSCI_EFF(_CLICK);

}

//抛弃不和谐的代码 
//void JetAndResManager::MyJettonOut(int code)
//{
//
//	//code 为0是玩家的操作
//	if (code == 0)
//	{
//		if (!m_bIsEnble) return;
//
//		if (m_bIsOpen)
//		{
//			m_targetIndex = Down;
//			m_bIsOpen = false;
//		}
//		else
//		{
//			m_targetIndex = Up;
//			m_bIsOpen = true;
//
//		}
//	}
//	else if (code == 1)
//	{
//		//打开
//		if (m_bIsOpen == false)
//		{
//			m_targetIndex = Up;
//			m_bIsOpen = true;
//		}
//	}
//	else if (code == 2)
//	{
//		//缩回
//		if (m_bIsOpen == true)
//		{
//			m_targetIndex = Down;
//			m_bIsOpen = false;
//		}
//	}
//
//	if (m_targetIndex == Down)
//	{
//		if (m_nowTime >= _ZERO)
//		{
//			setTitle(enumJetScene);
//		}
//		else if (m_countjettime >= _ZERO)
//		{
//			setTitle(enumCountJet);
//		}
//		else
//		{
//			setTitle(enumOpen);
//		}
//	}
//	else
//	{
//		if (m_nowTime >= _ZERO)
//		{
//			setTitle(enumPleaseJet);
//		}
//		else if (m_countjettime >= _ZERO)
//		{
//			setTitle(enumCountJet);
//		}
//		else
//		{
//			setTitle(enumGameScene);
//		}
//	}
//
//	moveTimeCountTojet(m_bIsOpen);
//
//	_PLAY_MUSCI_EFF(_CLICK);
//
//	if (!m_bIsRun)
//		schedule(schedule_selector(JetAndResManager::Running), 0.01f);
//
//}
////动画过程
//void JetAndResManager::Running(float t)
//{
//	m_bIsRun = true;
//
//	if (m_index != (int)m_targetIndex)
//	{
//		if (m_index > (int)m_targetIndex)
//		{
//			Move(m_moveNode, Vec2(_ZERO, -m_speed));
//			m_index--;
//		}
//		else
//		{
//			Move(m_moveNode, Vec2(_ZERO, m_speed));
//			m_index++;
//		}
//	}
//	else
//	{
//		/*if (m_targetIndex == Up)
//		{
//			m_jetimg->loadTextureNormal(Path_PNG_Jetton_Up);
//			m_jetimg->loadTexturePressed(Path_PNG_Jetton_Up);
//		}
//		else
//		{
//			m_jetimg->loadTextureNormal(Path_PNG_Jetton_Down);
//			m_jetimg->loadTexturePressed(Path_PNG_Jetton_Down);
//			
//		}*/
//		if (m_targetIndex == Down && m_nowTime > _ZERO)
//		{
//			TitleLabel::SetTitle(TitleLabel::enumTitleType::Normal1);
//		}
//		unschedule(schedule_selector(JetAndResManager::Running));
//		m_bIsRun = false;
//
//	}
//
//}
//

