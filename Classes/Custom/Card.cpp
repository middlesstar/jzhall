#include "Card.h"

using namespace cocos2d;

Card* Card::create(Sprite* front, Sprite* back)
{
	Card* pSprite = new Card();
	if (pSprite->initWithSprite(front, back))
	{
		pSprite->autorelease();
		return pSprite;
	}
	delete pSprite;
	return NULL;
}
bool Card::initWithSprite(Sprite* pFront, Sprite* pBack)
{
	if (pFront&&pBack&&Sprite::init())
	{
		flag = true;
		front = pFront;
		back = pBack;

		this->setContentSize(front->getContentSize());
		//卡片的反面
		back->setVisible(false);
		Vec2 pos = Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2);
		back->setPosition(pos);
		this->addChild(back);
		//卡片的正面
		front->setPosition(pos);
		this->addChild(front);
		return true;
	}
	return false;
}
//合上
void Card::close(float duration)
{
	if (front&&back)
	{
		front->stopAllActions();
		back->stopAllActions();
		auto orbitFront = OrbitCamera::create(duration, 1, 0, 90, -90, 0, 0);
		auto orbitBack = OrbitCamera::create(duration, 1, 0, 0 ,-90, 0, 0);
		back->setVisible(false);

		// 正面向右旋转90度->背面向左旋转90度
		front->runAction(Sequence::create(Show::create(), orbitBack, Hide::create(),
			TargetedAction::create(back, CCSequence::create(Show::create(), orbitFront, NULL)), NULL));
	}
	else{
	}
}
//打开
void Card::open(float duration)
{
	if (front&&back)
	{
		front->stopAllActions();
		back->stopAllActions();
		auto orbitFront = OrbitCamera::create(duration, 1, 0, 90, -90, 0, 0);
		auto orbitBack = OrbitCamera::create(duration, 1, 0, 0, -90, 0, 0);
		front->setVisible(false);

		// 正面向右旋转90度->背面向左旋转90度
		back->runAction(Sequence::create(Show::create(), orbitBack, Hide::create(),
			TargetedAction::create(front, CCSequence::create(Show::create(), orbitFront, NULL)), NULL));
	}
	else{
	}
}
void Card::move(float duration, float x, int num)
{
	auto act1 = MoveBy::create(duration, Vec2(x,0));
	MoveBy* act2;
	if (0 == num ){
		act2 = MoveBy::create(0.15 , Vec2(100, 0));
	}
	else if (1 == num)
	{
		act2 = MoveBy::create(0.15, Vec2(-100, 0));
	}
	else if (2 == num)
	{
		act2 = MoveBy::create(0.15, Vec2(30, 0));
	}
	else if (3 == num)
	{
		act2 = MoveBy::create(0.15, Vec2(-10, 0));
	}
	else if (4 == num)
	{
		act2 = MoveBy::create(0.15, Vec2(240, 0));
	}
	else if (5 == num)
	{
		act2 = MoveBy::create(0.15 , Vec2(-240,0));
	}
	
	auto seq = Sequence::create(act1, Repeat::create(Sequence::create(act2, act2->reverse(),NULL),10), act1->reverse(), NULL);
	this->runAction(seq);
}