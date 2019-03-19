#include "DrawSecond.h"
#include "shuihu/plazz/kernel/game/CClientKernel_SHZ.h"
#include "shuihu/View/Scene/LotteryScene/LotteryMission_SHZ.h"
#include "shuihu/View/Scene/getHouseIndex_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "network/CommandStructDef.h"
#include "Custom/Card.h"
#include "Player/Player.h"
#include "shuihu/Resource_SHZ.h"
using namespace cocos2d;

//���˳齱��ķɽ�һ���Ԫ��
#define  Lottery_coin_fly             "jinbiFly"
#define  Lottery_yuanbao_fly          "yuanbaoFly"
//������

#define GAME_NUMBER "bmfonts/game_number7.png"

//��ſ�Ƭ�ϼ�¼�ķ���
const int numb[6][6] = { { 100, 500, 1000, 5000, 1, 10 }
                       , { 1000, 5000, 10000, 50000, 10, 100 }
                       , { 5000, 25000, 50000, 200000, 50, 500 }
                       , { 10000, 50000, 100000, 400000, 100, 1000 }
                       , { 50000, 250000, 500000, 2000000, 500, 5000 }
                       , { 100000, 500000, 1000000, 5000000, 1000, 10000 } };
//�������Ƶ�λ��
const Vec2 pos[6] = { Vec2{ 207, 410 }
                      , Vec2{ 332, 410 }
					  , Vec2{ 457, 410 }
					  , Vec2{ 582, 410 }
					  , Vec2{ 707, 410 }
					  , Vec2{ 832, 410 } };
DrawSecond::DrawSecond()
{
	
}
DrawSecond::~DrawSecond()
{
	this->setTouchSwallow(false);
}
DrawSecond* DrawSecond::createLayer(int type)
{
	auto layer = new DrawSecond();
	if (layer && layer->init(type))
	{
		layer->autorelease();

		auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 180));
		layer->addChild(colorLayer);
		return layer;
	}
	delete layer;
}
bool DrawSecond::init(int type)
{
	if (!Layer::init())
	{
		return false;
	}
	srand(unsigned(time(NULL)));

	degree = type;
	//�����齱����
	s = Director::getInstance()->getWinSize();
	auto title = Sprite::createWithSpriteFrameName("draw_title.png");
	title->setPosition(Vec2(s.width/2,s.height-100));
	this->addChild(title,2);


	//����������
	contentLayer = Layer::create();
	contentLayer->setPosition(0, 0);
	this->addChild(contentLayer, 2);
	//����ʱ��ǩ
	t = 4;
	sprintf(timeShow, "%d", 5);
	time1 = LabelAtlas::create(timeShow, GAME_NUMBER, 36,50,'0');
	time1->setVisible(true);
	time1->setPosition(Vec2(s.width-230,s.height-200));
	contentLayer->addChild(time1, 3);


	
	//ʱ���ǩ
	this->schedule(schedule_selector(DrawSecond::timeChange1),1.0f);

	//�����齱��Ƭ
	this->createCard();

	//���ò�Ĵ�������
	this->setTouchSwallow(true);

	return true;
}
//���ò㼶����
void DrawSecond::setTouchSwallow(bool bSwallow/* = true*/)
{
	if (bSwallow)
	{

		m_pEventListener = EventListenerTouchOneByOne::create();
		m_pEventListener->setSwallowTouches(true);

		m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{ return true; };
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
	}
	else
	{
		getEventDispatcher()->removeEventListener(m_pEventListener);
		m_pEventListener = nullptr;
	}
}
//������Ƭ
void DrawSecond::createCard()
{
	char str[50];
	char number[50];
	for (int i = 0; i < 6; i++)
	{
		if (i < 4)
		{
			strcpy(str, "draw_card0.png");
		}
		else
		{
			strcpy(str, "draw_card1.png");
		}
		auto front = Sprite::createWithSpriteFrameName(str);
		auto back = Sprite::createWithSpriteFrameName("draw_card3.png");
		Card* sp = Card::create(front, back);
		sp->setPosition(pos[i]);
		sp->setTag(100 + i);
		contentLayer->addChild(sp, 3);
		cards.pushBack(sp);

		sprintf(number, "%d", numb[degree][i]);
		auto label = LabelTTF::create(number, "Arial", 25);
		label->setFontFillColor(Color3B(139, 69, 0));
		label->setPosition(Vec2(53, 70));
		label->setTag(10 + i);
		scoreLabel.pushBack(label);
		sp->addChild(label);
	}
}
//��������ʱ1
void DrawSecond::timeChange1(float f)
{
	if (t > -1)
	{
		sprintf(timeShow, "%d", t);
		time1->setString(timeShow);
		t--;
	}
	else
	{
		this->unschedule(schedule_selector(DrawSecond::timeChange1));
		time1->setVisible(false);
		this->privious();
	}
	
}
//��������ʱ
void DrawSecond::timeChange2(float f)
{
	if (t > -1)
	{
		sprintf(timeShow, "%d", t);
		time1->setString(timeShow);
		t--;
	}
	else
	{
		this->unschedule(schedule_selector(DrawSecond::timeChange2));
		time1->setVisible(false);
		int m = rand() % 6;
		for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
		{
			_PLAY_SOUND_SHZ(TURNCARD);
			Card* card = (*itor);
			if (card->getTag()-100 == m)
			{
				//��ת���еĿ�Ƭ
				int index = card->getTag() - 100;
				int lotteryIndex = mission->getLotteryExp().wIndex;
				Vec2 curPos = cards.at(index)->getPosition();

				cards.at(index)->setPosition(pos[lotteryIndex]);
				cards.at(lotteryIndex)->setPosition(curPos);
				cards.at(lotteryIndex)->open(0.5f);

				//label�濨Ƭһ����ת
				auto act = OrbitCamera::create(0.5f, 1, 0, 90, -90, 0, 0);
				auto sequ = Sequence::create(DelayTime::create(0.5), Show::create(), act, NULL);
				cards.at(lotteryIndex)->getChildByTag(lotteryIndex + 10)->runAction(sequ);

				//����δ��ѡ�п��ƵĹ�Ȧ
				for (Vector<Sprite* >::iterator itor1 = light.begin(); itor1 != light.end();)
				{
					Sprite* lights = (*itor1);
					if (lights->getTag()== index)
					{
						auto act1 = OrbitCamera::create(1.0f, 1, 0, 0, -180, 0, 0);
						lights->runAction(act1);
					}
					else
					{
						lights->removeFromParentAndCleanup(true);
					}
					++itor1;
				}

				//����δ��ѡ�еĿ�Ƭ
				for (int i = 0; i < 6; i++)
				{
					int n = rand() % 6;
					if (lotteryIndex == i || n == lotteryIndex || n == i)
					{
					}
					else
					{
						Vec2 onePos = cards.at(i)->getPosition();
						cards.at(i)->setPosition(cards.at(n)->getPosition());
						cards.at(n)->setPosition(onePos);
					}
					i++;
				}
				auto call = CallFunc::create(this, callfunc_selector(DrawSecond::showAll));
				auto call1 = CallFunc::create(this, callfunc_selector(DrawSecond::removeLayer));
				auto seq = Sequence::create(DelayTime::create(1.5f), call, DelayTime::create(2.5f), call1, NULL);
				this->runAction(seq);
			}
			else{
			}
			++itor;
		}
		
		dispatcher->removeEventListener(listener);
	}
}
//���ƶ������Ⱥ�
void DrawSecond::privious()
{
	mission = LotteryMission::getInstance();
	mission->reqLottery(1, degree);
	auto call1 = CallFunc::create(this, callfunc_selector(DrawSecond::shut));
	auto call2 = CallFunc::create(this, callfunc_selector(DrawSecond::control));
	auto call3 = CallFunc::create(this, callfunc_selector(DrawSecond::cardLight));
	auto seq = Sequence::create(call1, DelayTime::create(1.0f), call2, DelayTime::create(5.0f), call3, NULL);
	this->runAction(seq);

	
}
//����
void DrawSecond::shut()
{
	for (Vector<LabelTTF* >::iterator itor = scoreLabel.begin(); itor != scoreLabel.end();)
	{
		LabelTTF* label = (*itor);
		auto act1 = OrbitCamera::create(0.25f, 1, 0, 0, -90, 0, 0);
		auto seq = Sequence::create(act1, CallFunc::create( [label](){label->setVisible(false); }),NULL);
		label->runAction(seq);
		++itor;
	}
	for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
	{
		Card* card = (*itor);
		card->close(0.25);
		++itor;
	}
}

//����ϴ���ٶ�
void DrawSecond::control()
{
	int num = 5;
	for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
	{
		if (num > -1)
		{
			Card* card = (*itor);
			float x = s.width / 2 - card->getPositionX();
			card->move(0.9, x, num);
			num--;
		}
		++itor;
	}
}
//���Ʒ���
void DrawSecond::cardLight()
{
	for (int i = 0; i < 6; i++)
	{
		auto li = Sprite::createWithSpriteFrameName("draw_light.png"); 
		li->setTag(i);
		li->setPosition(Vec2(s.width / 2 - 330 + i * 125, s.height / 2 + 40));
		this->addChild(li,4);
		light.pushBack(li);
	}
	//����
	this -> touchlayer();
	t = 9;
	time1->setString("10");
	time1->setVisible(true);
	this->schedule(schedule_selector(DrawSecond::timeChange2), 1.0f);
}

void DrawSecond::touchlayer()
{	
	listener = EventListenerTouchOneByOne::create(); 
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool{
		this->m_ptPos = touch->getLocation();
		for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
		{			
			Card* card = (*itor);
			if (card->getBoundingBox().containsPoint(this->m_ptPos))
			{
				//��ת���еĿ�Ƭ
				int index = card->getTag() - 100;
				int lotteryIndex = mission->getLotteryExp().wIndex;
				log("------------lotteryIndex = %d----------",lotteryIndex);
				Vec2 curPos = cards.at(index)->getPosition();

 				cards.at(index)->setPosition(pos[lotteryIndex]);
 				cards.at(lotteryIndex)->setPosition(curPos);			
				cards.at(lotteryIndex)->open(0.5f);
				
				//label�濨Ƭһ����ת
				auto act = OrbitCamera::create(0.5f, 1, 0, 90, -90,0,0);
				auto sequ = Sequence::create(DelayTime::create(0.5), Show::create(), act, NULL);
				cards.at(lotteryIndex)->getChildByTag(lotteryIndex + 10)->runAction(sequ);

				//����ѡ�еĿ�������Ŀ���������Ȧ
				for (Vector<Sprite* >::iterator itor1 = light.begin(); itor1 != light.end();)
				{
					Sprite* lights = (*itor1);
					if (lights->getBoundingBox().containsPoint(this->m_ptPos))
					{
						auto act1 = OrbitCamera::create(1.0f, 1, 0, 0, -180, 0, 0);
						lights->runAction(act1);						
					}
					else
					{
						lights->removeFromParentAndCleanup(true);
					}
					++itor1;
				}

				//����δ��ѡ�еĿ�Ƭ
				for (int i = 0; i < 6; i++)
				{
					int m = rand() % 6;
					if (lotteryIndex == i || m == lotteryIndex || m == i)
					{
					}
					else
					{
						Vec2 onePos = cards.at(i)->getPosition();						
						cards.at(i)->setPosition(cards.at(m)->getPosition());
						cards.at(m)->setPosition(onePos);
					}
					i++;
				}
				auto call = CallFunc::create(this, callfunc_selector(DrawSecond::showAll));
				auto call1 = CallFunc::create(this, callfunc_selector(DrawSecond::removeLayer));
				auto seq = Sequence::create(DelayTime::create(1.5f), call, DelayTime::create(2.5f),call1, NULL);
				this->runAction(seq);
				
			}
			else{
			}
			++itor;
		}		
		return true;
	};
	
	listener->onTouchMoved = [this](Touch* touch, Event* event){};
	listener->onTouchEnded = [this](Touch* touche, Event* event)
	{
		_PLAY_SOUND_SHZ(TURNCARD);
 		for (Vector<Sprite* >::iterator itor = light.begin(); itor != light.end();)
 		{
 			Sprite* light = (*itor);
 			if (light->getBoundingBox().containsPoint(this->m_ptPos))
 			{
 				dispatcher->removeEventListener(listener);
 				this->unschedule(schedule_selector(DrawSecond::timeChange2));
 				time1->setVisible(false);
 			}
 			else
 			{
 			}
 			++itor;
 		}		
	};
	//����Ƭ��Ӵ�������
	dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, contentLayer);
}
//��ʾ���еĿ���
void DrawSecond::showAll()
{	
	for (Vector<Card* >::iterator itor = cards.begin(); itor != cards.end();)
	{
		Card* card = (*itor);
		int index = card->getTag() - 100;
		if (index == mission->getLotteryExp().wIndex)
		{
		}
		else
		{
			card->open(0.5f);
			
		}
		++itor;
	}
	
	for (Vector<LabelTTF* >::iterator itor = scoreLabel.begin(); itor != scoreLabel.end();)
	{
		
		LabelTTF* label = (*itor);
		int index = label->getTag() - 10;
		if (index == mission->getLotteryExp().wIndex)
		{

		}
		else
		{		
			//���Ʒ�ת
			auto act = OrbitCamera::create(0.5f, 1, 0, 90, -90, 0, 0);
			auto sequ = Sequence::create(DelayTime::create(0.5f), Show::create(), act, NULL);
			label->runAction(sequ);
		}
		++itor;
	}
}

//�Ƴ����еĲ� 
void DrawSecond::removeLayer()
{
	SHZGetHouseIndex::getInstance()->setJinBi(mission->getLotteryExp().lScore);
	//���þ���ֵ
	SHZGetHouseIndex::getInstance()->setExp(mission->getLotteryExp().dwExp);


	log("-------------mission->getLotteryExp().lScore = %d-----------------", mission->getLotteryExp().lScore);
	log("-------------mission->getLotteryExp().dwExp = %d-----------------", mission->getLotteryExp().dwExp);

	int index = mission->getLotteryExp().wIndex;
	int cardScore = numb[degree][index];
	if (index > 3)
	{
		//��¼��ȡ����Ԫ���ĸ���
		SHZGetHouseIndex::getInstance()->setLotteryBao(cardScore);
		G_NOTIFICATION_SHZ(Lottery_yuanbao_fly, nullptr);
	}
	else
	{
		//��¼��ȡ���Ľ�ҵĸ���
		SHZGetHouseIndex::getInstance()->setLotteryScore(cardScore);
		G_NOTIFICATION_SHZ(Lottery_coin_fly, nullptr);
	}

	Director::getInstance()->getRunningScene()->removeChildByTag(1000);
	
}