#include "shuihu/View/Scene/getHouseIndex_SHZ.h"
#include "DrawMain.h"
#include "DrawSecond.h"
#include "LanguageConfig.h"
#include "shuihu/Resource_SHZ.h"
USING_NS_CC;


//艺术字
#define GAME_NUMBER1 "bmfonts/game_number5.png"
#define GAME_NUMBER2 "bmfonts/game_number6.png"


//等级抽奖限制
const int degreeExpLimit[6] = {2000, 20000,100000, 200000,1000000,2000000 };
DrawMain::DrawMain() 
:tipText(nullptr)
, notDraw(nullptr)
, bt_start(nullptr)
{
	disScore = 0;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("draw/draw.plist");
}
DrawMain::~DrawMain()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("draw/draw.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("draw/draw.png");

	this->setTouchSwallow(this,false);
}
DrawMain* DrawMain::createLayer(int type, int exp, int index)
{
	auto layer = new DrawMain();
	if (layer && layer->init(type, exp,index))
	{
		layer->autorelease();

		auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 104));
		layer->addChild(colorLayer);
		return layer;
	}

	delete layer;

	
}
bool DrawMain::init(int type, int exp,int index)
{
	houseIndex = index;
	degree = type;
	lotteryExp = exp;
	originDegree = type;

	//距离下一等级的分数
	if (degree<5)
	{
		disScore = degreeExpLimit[degree + 1] - lotteryExp;
	}

	//存放卡片上记录的分数
	int numb[6][6] = { { 100, 500, 1000, 5000, 1, 10 }
	                 , { 1000, 5000, 10000, 50000, 10, 100 }
	                 , { 5000, 25000, 50000, 200000, 50, 500 }
	                 , { 10000, 50000, 100000, 400000, 100, 1000 }
	                 , { 50000, 250000, 500000, 2000000, 500, 5000 }
	                 , { 100000, 500000, 1000000, 5000000, 1000, 10000 } };
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			num[i][j] = numb[i][j];
		}
	}

	//背景图片
	s = Director::getInstance()->getWinSize();
	auto bg = Sprite::createWithSpriteFrameName("draw_panel.png");
	bg->setPosition(Vec2(s.width / 2, s.height / 2));
	this->addChild(bg, 2);

	//关闭按钮
	auto sp1 = Sprite::createWithSpriteFrameName("draw_close.png");
	auto bt_close = MenuItemSprite::create(sp1, sp1, CC_CALLBACK_1(DrawMain::onBtnClose, this));
	auto close = Menu::create(bt_close, NULL);
	close->setPosition(Vec2(s.width-90,s.height / 2 + 240));
	this->addChild(close, 4);

	//点击抽奖按钮
	auto sp2 = Sprite::createWithSpriteFrameName("draw_click.png");
	auto sp_start = MenuItemSprite::create(sp2, sp2, CC_CALLBACK_1(DrawMain::startPlay, this));
	bt_start = Menu::create(sp_start, NULL);
	bt_start->setPosition(Vec2(s.width - 220, 190));
	this->addChild(bt_start, 4);

	notDraw = Sprite::createWithSpriteFrameName("draw_click2.png");
	notDraw->setPosition(Vec2(s.width - 220, 190));
	notDraw->setVisible(false);
	this->addChild(notDraw,4);
	//抽奖等级显示
	degreeIcon = Sprite::createWithSpriteFrameName("draw_down1.png");
	degreeIcon->setPosition(Vec2(320,185));
	this->addChild(degreeIcon, 4);
	this->showDegree();


	//当前积分
	char expScore[50];
	sprintf(expScore,"%d",exp);
	curScore = LabelAtlas::create(expScore, GAME_NUMBER1,18,23,'0');
	curScore->setPosition(Vec2(s.width/2-155,s.height / 2 + 147));
	this->addChild(curScore,4);


	//显示提示信息您当前可进行更高等级的抽奖
	tipText = Sprite::createWithSpriteFrameName("draw_text.png");
	tipText->setPosition(Vec2(547, 190));
	this->addChild(tipText,4);

	//tipText = Label::create(GETSTRING(1501), "Arial", 30, CCSizeMake(300, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	//tipText->setVisible(true);
	//tipText->setPosition(Vec2(347, 170));
	//this->addChild(tipText, 10);

	//抽奖限制分数
	char limitExp[50];
	//if (5 == type)
	//{
		sprintf(limitExp, "%d", degreeExpLimit[type]);
	//}
	//else
	//{
	//	sprintf(limitExp, "%d", degreeExpLimit[type+1]);
	//}
	
	limitScore = LabelAtlas::create(limitExp, GAME_NUMBER1, 18,23,'0');
	limitScore->setPosition(Vec2(s.width/2+225, s.height / 2 + 147));
	this->addChild(limitScore,4);

	
	//创建领奖进度条
	this->createProgress(type,exp);
	//添加卡片
	this->createCards();
	
	//添加等级选择
	this->createDegree(type,exp);

	//设置层的触摸屏蔽
	this->setTouchSwallow(this, true);

	return true;
}

void DrawMain::showDegree()
{
	//切换抽奖等级图片
	char str[50];
	if (degree < 5)
	{
		if (degree == originDegree)
		{
			degreeIcon->setVisible(true);
			sprintf(str, "draw_down%d.png", degree + 1);
		}
		else if (degree < originDegree)
		{
			//提示可以有更高等级的抽奖并把点击抽奖进度条等内容隐藏
			degreeIcon->setVisible(false);
			sprintf(str, "draw_down%d.png", degree+1);
		}
		else if (degree>originDegree)
		{
			//显示到该等级的抽奖以及进度条显示到该等级的差距
			degreeIcon->setVisible(true);
			sprintf(str, "draw_down%d.png", degree);
		}
	}
	else
	{
		degreeIcon->setVisible(true);
		sprintf(str, "draw_down%d.png", degree);
	}

	degreeIcon->setSpriteFrame(str);

}
void DrawMain::setTouchSwallow(cocos2d::Object* psender, bool bSwallow)
{
	auto objLayer = (Layer *)psender;
	if (bSwallow)
	{

		m_pEventListener = EventListenerTouchOneByOne::create();
		m_pEventListener->setSwallowTouches(true);

		m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool{ return true; };
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, objLayer);
	}
	else
	{
		getEventDispatcher()->removeEventListener(m_pEventListener);
		m_pEventListener = nullptr;
	}
}
//创建领奖进度条
void DrawMain::createProgress(int type, int exp)
{
	progressBg = Sprite::createWithSpriteFrameName("draw_bar0.png");
	progressBg->setPosition(Vec2(s.width / 2 + 30, 185));
	this->addChild(progressBg, 4);

	d_progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("draw_bar1.png"));
	d_progressBar->setType(kCCProgressTimerTypeBar);
	d_progressBar->setPosition(Vec2(133, 20));
	d_progressBar->setPercentage(0); 
	d_progressBar->setMidpoint(Vec2(0, 0));
	d_progressBar->setBarChangeRate(Vec2(1, 0));

	//创建进度条百分比显示	
	perLabel = LabelAtlas::create("", GAME_NUMBER2, 14, 22, '.');
	perLabel->setPosition(Vec2(130, 18));
	perLabel->setAnchorPoint(Vec2(0.5,0.5));
	progressBg->addChild(perLabel, 2);

	//金币
	auto gold = Sprite::createWithSpriteFrameName("draw_gold.png");
	gold->setPosition(Vec2(-20, 20));
	progressBg->addChild(gold, 4);

	this->changeProgress();
	progressBg->addChild(d_progressBar);

	
}
//创建进度条改变
void DrawMain::changeProgress()
{
	ProgressTo* to;
	if (5 == degree)
	{
		if (originDegree < degree)
		{
			bt_start->setVisible(false);
			notDraw->setVisible(true);
			tipText->setVisible(false);
			progressBg->setVisible(true);
		}
		else
		{
			bt_start->setVisible(true);
			notDraw->setVisible(false);
			tipText->setVisible(false);
			progressBg->setVisible(true);

		}		
		if (lotteryExp >= degreeExpLimit[degree])
		{
			to = ProgressTo::create(1, 100);
			char bit[100];
			sprintf(bit, "%d/%d", 100, 100);
			perLabel->setString(bit);
		}
		else
		{
			to = ProgressTo::create(1, lotteryExp * 100 / degreeExpLimit[degree]);

			char bit[100];
			sprintf(bit, "%lu/%lu", lotteryExp, degreeExpLimit[degree]);
			perLabel->setString(bit);
		}
	}
	else
	{
		if (degree == originDegree)
		{
			bt_start->setVisible(true);
			notDraw->setVisible(false);
			tipText->setVisible(false);
			progressBg->setVisible(true);
			to = ProgressTo::create(1, lotteryExp * 100 / degreeExpLimit[degree + 1]);

			char bit[100];
			sprintf(bit, "%lu/%lu", lotteryExp, degreeExpLimit[degree + 1]);
			perLabel->setString(bit);
		}
		else if (degree < originDegree)
		{
			//隐藏进度条和改等级的抽奖并提示能进行更高等级的抽奖
			bt_start->setVisible(false);
			notDraw->setVisible(false);
			progressBg->setVisible(false);
			tipText->setVisible(true);

			to = ProgressTo::create(1, 100);
			char bit[100];
			sprintf(bit, "%d/%d", 100, 100);
			perLabel->setString(bit);
			
		}
		else if (degree>originDegree)
		{
			tipText->setVisible(false);
			bt_start->setVisible(false);
			notDraw->setVisible(true);
			//显示到该等级的抽奖
			progressBg->setVisible(true);
			to = ProgressTo::create(1, lotteryExp * 100 / degreeExpLimit[degree]);

			char bit[100];
			sprintf(bit, "%lu/%lu", lotteryExp, degreeExpLimit[degree]);
			perLabel->setString(bit);
		}
		//if (lotteryExp >= degreeExpLimit[degree + 1])
		//{
		//	to = ProgressTo::create(1, 100);

		//	char bit[100];
		//	sprintf(bit, "%lu/%lu", 100, 100);
		//	perLabel->setString(bit);
		//}
		//else
		//{
		//	to = ProgressTo::create(1, lotteryExp * 100 / degreeExpLimit[degree + 1]);

		//	char bit[100];
		//	sprintf(bit, "%lu/%lu", lotteryExp, degreeExpLimit[degree + 1]);
		//	perLabel->setString(bit);
		//}

	}

	d_progressBar->runAction(to);
	
}

//抽奖限制积分改变
void DrawMain::changeScoreLimit()
{
	char limitExp[50];
	//if (5 == degree)
	//{
		sprintf(limitExp, "%d", degreeExpLimit[degree]);
	//}
	//else
	//{
	//	sprintf(limitExp, "%d", degreeExpLimit[degree + 1]);
	//}
	limitScore->setString(limitExp);

}
void DrawMain::createCards()
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
		sprintf(number, "%d", num[degree][i]);
		auto card = Sprite::createWithSpriteFrameName(str);
		card->setPosition(Vec2(s.width / 2 - 245 + i * 120, s.height / 2 + 10));
		this->addChild(card, 3);

		auto label = LabelTTF::create(number, "Arial", 25);
		label->setFontFillColor(Color3B(139,69,0));
		label->setPosition(Vec2(53, 70));
		scoreLabel.pushBack(label);
		card->addChild(label);

	}
}

//创建抽奖等级
void DrawMain::createDegree(int type, int exp)
{	
	char str1[100];
	char str2[100];
	for (int i = 1; i < 7; i++)
	{
		sprintf(str1, "draw_left_1%d.png", i);
		sprintf(str2, "draw_left_0%d.png", i);
		auto sp1 = Sprite::createWithSpriteFrameName(str1);
		auto sp2 = Sprite::createWithSpriteFrameName(str2);
		auto normal_Item = MenuItemSprite::create(sp1,sp1);
		auto select_Item = MenuItemSprite::create(sp2,sp2); 
		auto item = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DrawMain::menuItemCallback, this), normal_Item, select_Item,NULL);
		item->setTag(100 + i);
		if ((type+1) == i)
		{
			item->setSelectedIndex(1);
		}
		else
		{
			item->setSelectedIndex(0);
		}
		menuToggle.pushBack(item);
		item->setPosition(Vec2(150,s.height/2+170-77*(i-1)));
	}
	auto menu = Menu::create(menuToggle.at(0), menuToggle.at(1), menuToggle.at(2), menuToggle.at(3), menuToggle.at(4), menuToggle.at(5), NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 4);	
}
//抽奖等级选择
void DrawMain::menuItemCallback(cocos2d::Object* psender)
{
	 
	auto item = (MenuItemToggle *)psender;
	int tag = item->getTag();
	degree = tag - 101;
	int selectIndex;
	MenuItemToggle* target_Item;
	for (int i = 1; i < 7; i++)
	{
		target_Item = menuToggle.at(i-1);
		if (tag == target_Item->getTag())
		{
			if (target_Item->getSelectedIndex() == 0)
			{
				target_Item->setSelectedIndex(1);
				this->degreeUI(target_Item);
				this->changeProgress();
				this->changeScoreLimit();
			}
			else
			{
				this->degreeUI(target_Item);
				this->changeProgress();
				this->changeScoreLimit();
			}			
		}
		else
		{
			if (target_Item->getSelectedIndex() == 1)
			{
				target_Item->setSelectedIndex(0);
			}
		}
	}	
}
//界面等级显示
void DrawMain::degreeUI(cocos2d::Object* psender)
{
	auto item = (MenuItemToggle *)psender;
	int tag = item->getTag() - 101;
	//更改卡牌显示数据
	char number[50];
	for (int i = 0; i < 6; i++)
	{
		sprintf(number, "%d", num[tag][i]);		
		scoreLabel.at(i)->setString(number);
		
	}	
	this->showDegree();

}  
//点击抽奖
void DrawMain::startPlay(cocos2d::Object* psender)
{
	_PLAY_SOUND_SHZ(PRESS);
	//判断该房间是否可以抽奖
	if (houseIndex > 0 && (0 == degree))
	{
		
	}
	else if (houseIndex > 1 && (degree < 3))
	{
		
	}
	else
	{
		//判断该等级是否可以抽奖
		if (lotteryExp < degreeExpLimit[degree])
		{
			expCue = LabelTTF::create("12334545", "Arial", 30);
			//没有设置位置
			this->addChild(expCue, 4);
		}
		else
		{
			this->lotteryLayer();
		}

	}
	
		
}

void DrawMain::lotteryLayer()
{
	if (5 == degree)
	{
		auto startLayer = DrawSecond::createLayer(degree);
		startLayer->setPosition(Vec2::ZERO);
		//Director::getInstance()->getRunningScene()->addChild(startLayer, 1000);
		this->addChild(startLayer, 5);
		//移除第一个界面
		//this->removeFromParentAndCleanup(true);
	}
	else
	{
		if (disScore > 0)
		{
			showLayer = LayerColor::create(Color4B(0, 0, 0, 104));
			showLayer->setPosition(Vec2(0, 0));
			this->addChild(showLayer, 6);

			this->setTouchSwallow(showLayer, true);

			auto bgBoard = Sprite::createWithSpriteFrameName("draw_little.png");
			bgBoard->setPosition(Vec2(s.width / 2, s.height / 2));
			showLayer->addChild(bgBoard);

			char distScore[50];
			sprintf(distScore, "%d", disScore);
			auto label = LabelTTF::create(distScore, "Arial", 28);
			label->setFontFillColor(Color3B(0,255,0));
			label->setPosition(Vec2(340, 200));
			bgBoard->addChild(label);

			ControlButton*	okBtn = createButton_SHZ("draw_yes.png", 0, 0, this, cccontrol_selector(DrawMain::onBtnClick));
			okBtn->setPosition(Vec2(130, 60));
			okBtn->setTag(101);
			bgBoard->addChild(okBtn);

			ControlButton*	cancelBtn = createButton_SHZ("draw_no.png", 0, 0, this, cccontrol_selector(DrawMain::onBtnClick));
			cancelBtn->setPosition(Vec2(330, 60));
			cancelBtn->setTag(102);

			bgBoard->addChild(cancelBtn);
		}
		else
		{
			auto startLayer = DrawSecond::createLayer(degree);
			startLayer->setPosition(Vec2::ZERO);
			//Director::getInstance()->getRunningScene()->addChild(startLayer, 1000);
			this->addChild(startLayer, 5);
			//移除第一个界面
			//this->removeFromParentAndCleanup(true);
		}
		
	}

}

void DrawMain::onBtnClick(cocos2d::Object* psender, cocos2d::extension::Control::EventType e)
{
	_PLAY_SOUND_SHZ(OTHERPRESS);
	ControlButton* btn = (ControlButton*)psender;
	if (101 == btn->getTag())
	{
		auto startLayer = DrawSecond::createLayer(degree);
		startLayer->setPosition(Vec2::ZERO);
		this->addChild(startLayer, 5);

		showLayer->removeFromParentAndCleanup(true);
		//同步确定抽奖后的经验值
		char currentScore[50];
		//lotteryExp = lotteryExp - degreeExpLimit[degree];
		lotteryExp = 0;
		sprintf(currentScore, "%d", lotteryExp);
		curScore->setString(currentScore);
		SHZGetHouseIndex::getInstance()->setExp(lotteryExp);
        
		this->changeProgress();
	}
	else
	{
		showLayer->removeFromParentAndCleanup(true);
	}


}
void DrawMain::onBtnClose(Object* psender)
{
	_PLAY_SOUND_SHZ(OTHERPRESS);
	this->removeFromParentAndCleanup(true);
}