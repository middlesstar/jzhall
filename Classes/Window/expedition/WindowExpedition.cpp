#include "WindowExpedition.h"
#include "LogicExpedition.h"
#include "LanguageConfig.h"
#include "Player/Player.h"
#include "Util/GameUtil.h"
#include "Custom/Card.h"

//艺术字
#define TEXT_NUMBER "images/number/number_1.png"
#define TIMER_NUMBER "images/number/number_13.png"

//资源路径
#define SOURCE_PLIST "images/hall/expedition/cards.plist"
#define SOURCE_PNG   "images/hall/expedition/cards.png"

WindowExpedition::WindowExpedition()
{
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(SOURCE_PLIST);
	
}
WindowExpedition::~WindowExpedition()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(SOURCE_PLIST);
	Director::getInstance()->getTextureCache()->removeTextureForKey(SOURCE_PNG);
	
}
void WindowExpedition::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZExpedition.csb");
	CCASSERT(pMainNode, "WindowExpedition::setContent() layout/JZExpedition.json load nullptr!");


	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	contentLayer = Layer::create();
	this->setPosition(Vec2(0,0));
	this->addChild(contentLayer,1);

	m_bg1 = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "expedition_bg1"));
	m_bg2 = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "expedition_bg2"));
	ticket = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "ticket"));
	tips = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "tips"));
	clock = dynamic_cast<Sprite*>(_seekNodeByName(pMainNode, "clock"));

	m_pBtnOpen = dynamic_cast<Button*>(_seekNodeByName(pMainNode, "btn_open"));
	m_pBtnChallenge = dynamic_cast<Button*>(_seekNodeByName(pMainNode, "btn_challenge"));

	//显示船票数量
	text = LabelAtlas::create("123",TEXT_NUMBER,17,21,'0');
	text->setPosition(Vec2(600,188));
	m_bg2->addChild(text,2);

	//显示倒计时的label
	timer = LabelAtlas::create("10", TIMER_NUMBER, 25, 36, '0');
	timer->setAnchorPoint(Vec2(0.5,0.5));
	timer->setPosition(Vec2(625,263));
	timer->setVisible(false);
	m_bg2->addChild(timer, 2);

	//设置按钮关闭事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		contentLayer->removeFromParentAndCleanup(true);
		this->close();
	});

	//设置按钮开始远征事件
	_addButtonClickEvent(pMainNode, "btn_open", CC_CALLBACK_1(WindowExpedition::_btnOpen_CallBack, this));
	_addButtonClickEvent(pMainNode, "btn_challenge", CC_CALLBACK_1(WindowExpedition::_btnChallenge_CallBack, this));

}

void WindowExpedition::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
}


void WindowExpedition::updateWindow(int nCode, void* pData)
{
	
}

//开启远征回调事件
void WindowExpedition::_btnOpen_CallBack(Ref* pRef)
{
	//m_bg1->setVisible(false);
	//m_bg2->setVisible(true);

	//this->creareCard();
	HallGameScene::getInstance()->openMessageTip(4);
}

//创建卡牌
void WindowExpedition::creareCard()
{
	
	for (int i = 1; i < 4; i++)
	{
		auto front = Sprite::createWithSpriteFrameName(StringUtils::format("block_%d.png", i));
		auto back = Sprite::createWithSpriteFrameName("back.png");
		Card* sp = Card::create(back, front);
		sp->setPosition(Vec2(287+(i-1)*170,300));
		sp->setTag(100 + i);
		contentLayer->addChild(sp, 2);
		cards.pushBack(sp);
	}
}

//开始挑战回调事件
void WindowExpedition::_btnChallenge_CallBack(Ref* pRef)
{
	m_pBtnChallenge->setVisible(false);
	tips->setVisible(false);
	clock->setVisible(true);
	timer->setVisible(true);


	auto call1 = CallFunc::create(this, callfunc_selector(WindowExpedition::turnTwo));
	auto callTime = CallFunc::create([this](){
		//时间标签
		time = 9;
		this->schedule(schedule_selector(WindowExpedition::timeChange), 1.0f);
	});
	auto callTouch = CallFunc::create(this, callfunc_selector(WindowExpedition::touchlayer));
	auto callHand = CallFunc::create(this, callfunc_selector(WindowExpedition::handShake));
	auto spa = Spawn::create(callTime, callTouch, callHand, NULL);
	auto seq = Sequence::create(call1, DelayTime::create(1.0f), spa, NULL);
	this->runAction(seq);


}

//创建倒计时改变
void WindowExpedition::timeChange(float f)
{
	if (time > -1)
	{
		timer->setString(StringUtils::format("%d", time));
		time--;
	}
	else
	{
		this->unschedule(schedule_selector(WindowExpedition::timeChange));
		timer->setVisible(false);
		clock->setVisible(false);
		this->autoTurn();
		dispatcher->removeEventListener(listener);
		hand->stopAllActions();
		hand->removeFromParentAndCleanup(true);
		this->showResult();
	}
}

//翻转前两个卡牌
void WindowExpedition::turnTwo()
{
	for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
	{
		Card* card = (*itor);
		
		if (card->getTag() < 103)
		{
			card->close(0.5);
		}

		++itor;
	}
}

//自动翻转卡牌
void WindowExpedition::autoTurn()
{
	for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
	{
		Card* card = (*itor);
		
		if (103 == card->getTag())
		{
			card->close(0.5);
		}
		
		++itor;
	}
}

//添加触摸
void WindowExpedition::touchlayer()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
		this->m_ptPos = touch->getLocation();
		for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
		{
			if ((*itor)->getBoundingBox().containsPoint(this->m_ptPos))
			{
				if (103 == (*itor)->getTag())
				{
					(*itor)->close(0.5);
					dispatcher->removeEventListener(listener);
					this->unschedule(schedule_selector(WindowExpedition::timeChange));
					timer->setVisible(false);
					clock->setVisible(false);
					hand->stopAllActions();
					hand->removeFromParentAndCleanup(true);
					this->showResult();
				}
				else{
				}
			}
			itor++;
			
		}
		return true;
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event){};
	listener->onTouchEnded = [this](Touch* touch, Event* event){
		
	};
	//给卡片添加触摸监听
	dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, contentLayer);
}

//创建晃动的手
void WindowExpedition::handShake()
{
	hand = Sprite::createWithSpriteFrameName("expedition_hand.png");
	hand->setPosition(Vec2(676,220));
	contentLayer->addChild(hand, 3);

	auto move = MoveBy::create(0.3,Vec2(10,-10));
	auto seq = Sequence::create(move, move->reverse(), NULL);
	hand->runAction(RepeatForever::create(seq));
}

//显示开奖结果
void WindowExpedition::showResult()
{
	Size size = Director::getInstance()->getWinSize();
	show = Sprite::createWithSpriteFrameName("expedition_mask.png");
	show->setPosition(Vec2(size.width/2-70,size.height/2-60));
	contentLayer->addChild(show, 3);

	Sprite* showText = Sprite::createWithSpriteFrameName("expedition_5.png");
	showText->setPosition(Vec2(show->getContentSize().width/2,show->getContentSize().height/2));
	show->addChild(showText);
	this->scheduleOnce(schedule_selector(WindowExpedition::removeShow), 2.0f);
}

//移除开奖结果的显示
void WindowExpedition::removeShow(float t)
{
	show->removeFromParentAndCleanup(true);
}