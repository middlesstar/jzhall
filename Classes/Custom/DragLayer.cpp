#include "DragLayer.h"

DragLayer::DragLayer()
	: m_fXMax(0)
	, m_fXMin(0)
	, m_fYMax(0)
	, m_fYMin(0)
{

}

DragLayer::~DragLayer()
{

}

bool DragLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_setTouchListener();

	return true;
}

bool DragLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (!_visible)
	{
		return false;
	}

	Vec2 pBeginPos = touch->getLocation();
	m_posOffset = pBeginPos - getParent()->convertToWorldSpace(_position);

	Vec2 srcPos = Vec2(_position.x - _contentSize.width * _anchorPoint.x, _position.y - _contentSize.height * _anchorPoint.y);
	srcPos = getParent()->convertToWorldSpace(srcPos);
	Rect rect = Rect(srcPos, _contentSize);

	return rect.containsPoint(pBeginPos);
}

void DragLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	Vec2 desPos = touch->getLocation() - m_posOffset;
	desPos.x = MIN(desPos.x, m_fXMax);
	desPos.x = MAX(desPos.x, m_fXMin);
	desPos.y = MIN(desPos.y, m_fYMax);
	desPos.y = MAX(desPos.y, m_fYMin);

	this->setPosition(desPos);
}

void DragLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void DragLayer::setContentSize(const Size& contentSize)
{
	Layer::setContentSize(contentSize);

	Size size = Director::getInstance()->getWinSize();
	m_fXMax = size.width - (1 - _anchorPoint.x) * contentSize.width;
	m_fXMin = _anchorPoint.x * contentSize.width;
	m_fYMax = size.height - (1 - _anchorPoint.y) * contentSize.height;
	m_fYMin = _anchorPoint.y * contentSize.height;
}

void DragLayer::openDebug()
{
	LayerColor* ply = LayerColor::create();
	ply->setContentSize(this->getContentSize());
	ply->setColor(ccc3(255, 0, 0));
	ply->setOpacity(255);
	ply->setAnchorPoint(Vec2(0.5, 0.5));
	ply->setPosition(Vec2(-getContentSize().width / 2, -getContentSize().height / 2));
	this->addChild(ply);
}

void DragLayer::_setTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(DragLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DragLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DragLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}