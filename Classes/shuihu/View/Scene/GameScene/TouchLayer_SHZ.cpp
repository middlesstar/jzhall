#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "TouchLayer_SHZ.h"

USING_NS_CC;

SHZTouchLayer* SHZTouchLayer::create(int priority)
{
	SHZTouchLayer* layer = new SHZTouchLayer();
	if (layer && layer->init(priority))
	{
		layer->autorelease();
		return layer;
	}

	delete layer;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
SHZTouchLayer::SHZTouchLayer()
	: mIsTouchEnabled(false)
	, mITouchSink(0)
{
}

SHZTouchLayer::~SHZTouchLayer()
{
	setTouchEnabled(false);
}

//初始化方法
bool SHZTouchLayer::init(int priority)
{
	do 
	{
		mPriority=priority;
		CC_BREAK_IF(!CCLayer::init());
		return true;
	} while (0);

	return false;
}

void SHZTouchLayer::setTouchEnabled(bool flag)
{
	if (flag)
	{

		m_pEventListener = EventListenerTouchOneByOne::create();
		m_pEventListener->setSwallowTouches(true);

		m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{return true; };
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
	}
	else
	{
		getEventDispatcher()->removeEventListener(m_pEventListener);
		m_pEventListener = nullptr;
	}
}

void SHZTouchLayer::setSink(SHZITouchSink* pITouchSink)
{
	mITouchSink = pITouchSink;
}

