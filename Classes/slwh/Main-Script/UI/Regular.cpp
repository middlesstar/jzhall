#include "Regular.h"


//节点下逻辑标签
#define	BK_REGULAR_IMAGE_NAME	"regular"

//BK_REGULAR_IMAGE下子节点
#define	CLOSE_BT_NAME			"closeBt"


#define	_FADEIN				0.3f
#define	_FADEOUT			0.3f


Regular* Regular::m_instance = nullptr;
Regular::Regular() :
m_layerColor(nullptr),
m_close(nullptr),
m_fadein(nullptr),
m_fadeout(nullptr),
m_bIsCloseRunning(false),
m_bIsInit(false)
{
}


Regular::~Regular()
{
	if (nullptr != m_fadein)
		m_fadein->release();

	if (nullptr != m_fadeout)
		m_fadeout->release();

	m_instance = nullptr;
	m_bIsInit = false;
	m_layerColor = nullptr; 
	m_fadein = nullptr;
	m_fadeout = nullptr;
	m_close = nullptr;
}


void Regular::Start()
{

	m_close = dynamic_cast<Button*>(m_node->getChildByName(BK_REGULAR_IMAGE_NAME)->getChildByName(CLOSE_BT_NAME));
	CC_ASSERT(nullptr != m_close);
	m_close->addTouchEventListener(this, toucheventselector(Regular::CloseEvent));

	//颜色层
	Vec2 visibleSize = Director::getInstance()->getWinSize();
	m_layerColor = CCLayerColor::create(Color4B(_ZERO, _ZERO, _ZERO, COLOR_LAYER_TANSPARENT));
	m_layerColor->setContentSize(Size(visibleSize.x, visibleSize.y));
	m_layerColor->ignoreAnchorPointForPosition(false);
	m_layerColor->setPosition(visibleSize / 2);
	m_node->addChild(m_layerColor, COLOR_LAYER_LEVEL);
	m_layerColor->setTouchEnabled(true);
	

	m_fadein = CCFadeIn::create(_FADEIN);
	CC_ASSERT(nullptr != m_fadein);
	m_fadein->retain();

	m_fadeout = CCSequence::create(
		CCFadeOut::create(_FADEOUT),
		CCCallFunc::create(this, callfunc_selector(Regular::CallBackClose)),
		nullptr
		);
	CC_ASSERT(nullptr != m_fadeout);
	m_fadeout->retain();

	m_node->setVisible(false);
	m_node->setOpacity(_ZERO);
	m_node->setZOrder(500);
}


bool Regular::onTouchBegan(Touch* touch, Event* event)
{

	return true;
}


void Regular::CloseEvent(Ref* pSender, TouchEventType type)
{
	if (true == m_bIsCloseRunning)
		return;

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		((Node*)pSender)->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		Hide();	
		break;
	case TOUCH_EVENT_CANCELED:
		((Node*)pSender)->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	default:
		break;
	}
}


Regular* Regular::GetInstance()
{
	if (nullptr == m_instance)
		m_instance = Regular::create();

	return m_instance;
}

void Regular::Show()
{
	if (nullptr == m_node)
		return;
	
	m_node->setVisible(true);

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Regular::onTouchBegan, this);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_layerColor);
	//m_node->set

	m_node->runAction(m_fadein->clone());
}

void Regular::Hide()
{
	if (nullptr == m_node)
		return;

	m_bIsCloseRunning = true;
	_PLAY_MUSCI_EFF(_CLICK);
	m_node->stopAllActions();
	m_node->runAction(m_fadeout->clone());
}


void Regular::CallBackClose()
{
	m_bIsCloseRunning = false;
	m_node->setVisible(false);
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->removeEventListenersForTarget(m_layerColor);
}



