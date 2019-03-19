#include "WindowFMSign.h"
#include "LogicFreeMoney.h"
#include "cocos2d.h"

#define GOLD_TEXT       "freegold_gold.png"
#define GOLD_GET        "freegold_get.png"
#define GOLD_BOX0       "freegold_box0.png"
#define GOLD_BOX1       "freegold_box1.png"
#define GOLD_BOX2       "freegold_box2.png"
#define GOLD_BOX3       "freegold_box3.png"
#define GOLD_LEFT       "freegold_left.png"
#define GOLD_NUMBER     "images/number/number_12.png"
#define GOLD_SQUARE1    "freegold_square1.png"
#define GOLD_SQUARE2    "freegold_square2.png"
#define GOLD_VIP        "freegold_vip.png"
#define GOLD_RIGHT      "freegold_right.png"


#define SCROLL_VIEW_WIDTH			    561
#define SCROLL_VIEW_HEIGHT			    400
#define SCROLL_VIEW_LINE_HEIGHT		    110
#define SIGN_ROW					    6
#define SIGN_COL					    5

#define SIGN_DAY_BLOCK_TAG			    1
#define BLOCK_TAG					    3
#define SIGN_DAY_TAG_BASE			    10
#define CONTINUE_SIGN_BOX_ACTION_TAG	1
#define CONTINUE_SIGN_BOX_TAG		    2

static ControlButton* createButton_SHZ(const char* normal, const char* selected, const char* disable, CCObject* target, cocos2d::extension::Control::Handler handle)
{
	ui::Scale9Sprite* sptNormal = ui::Scale9Sprite::createWithSpriteFrameName(normal);
	ui::Scale9Sprite* sptSelected = selected == 0 ? 0 : ui::Scale9Sprite::createWithSpriteFrameName(selected);
	ui::Scale9Sprite* sptDisable = disable == 0 ? 0 : ui::Scale9Sprite::createWithSpriteFrameName(disable);

	//关闭按钮
	ControlButton* bt = ControlButton::create();
	bt->setBackgroundSpriteForState(sptNormal, Control::State::NORMAL);
	if (sptSelected)
		bt->setBackgroundSpriteForState(sptSelected, Control::State::HIGH_LIGHTED);
	if (sptDisable)
		bt->setBackgroundSpriteForState(sptDisable, Control::State::DISABLED);
	bt->setPreferredSize(sptNormal->getPreferredSize());
	bt->addTargetWithActionForControlEvents(target, handle, Control::EventType::TOUCH_UP_INSIDE);
	return bt;
}



static const int goldArray[][5] = {
	{ 3000, 5000, 8000, 10000, 30000 },
	{ 3000, 5000, 8000, 10000, 40000 },
	{ 3000, 5000, 8000, 10000, 50000 },
	{ 3000, 5000, 8000, 10000, 60000 },
	{ 3000, 5000, 8000, 10000, 70000 },
	{ 3000, 5000, 8000, 10000, 200000 }
};


WindowFMSign::WindowFMSign()
    : m_scrollView(nullptr)
    , m_continueSignBtn(nullptr)
    , m_labelContinue(nullptr)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hall/dialysign/freegold.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hall/dialysign/block.plist");
}

WindowFMSign::~WindowFMSign()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("images/hall/dialysign/freegold.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("images/hall/dialysign/block.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("images/hall/dialysign/freegold.png");
	Director::getInstance()->getTextureCache()->removeTextureForKey("images/hall/dialysign/block.png");
}

void WindowFMSign::setContent()
{
	pMainNode = _loadCSBLayoutFile("layout/JZFMSign.csb");
	CCASSERT(pMainNode, "WindowFMSign::setContent() layout/JZFMSign.json load nullptr!");

	m_pNode->addChild(pMainNode);
	this->addBlackLayer();

	//显示每日签到界面的布局
	this->showView();
	onQueryDailySign();

	//设置按钮事件
	_addButtonClickEvent(pMainNode, "btn_close", [this](Ref* sender)
	{
		this->close();
	});
}

void WindowFMSign::onEnter()
{
	WindowBase::onEnter();

	ScaleIn();
	
}
//进入签到界面设置各个数值
void WindowFMSign::onQueryDailySign()
{
	LogicFreeMoney* pLogic = LogicFreeMoney::getInstance();

	int signIndex = pLogic->getSignDay().signindex;
	int continueLogin = pLogic->getSignDay().lxcount;
	//判断是否已经签过
	int flag1 = pLogic->getSignDay().gold;
	int flag2 = pLogic->getSignDay().ret;

		if (0 == flag1&& 0 == flag2)
		{ 
			if (1 == signIndex)
			{

			}
			else
			{
				for (int i = 1; i <= signIndex - 1; ++i)
				{
					addGotSign(SIGN_DAY_TAG_BASE + i - 1);
				}
			}
			setDailySignButtonEnabled(SIGN_DAY_TAG_BASE + signIndex - 1, true);
			addBlockAnimation(SIGN_DAY_TAG_BASE + signIndex - 1);
		
		}
		else
		{		
			for (int i = 1; i <= signIndex - 1; ++i)
			{
				addGotSign(SIGN_DAY_TAG_BASE + i - 1);
			}
		}	



	if (continueLogin >= 7)
	{
		m_labelContinue->setString(StringUtils::format("%d", 7));
		setContinueSignButtonEnabled(true);
	}
	else
	{
		m_labelContinue->setString(StringUtils::format("%d", continueLogin));
	}
}

//显示每日签到界面的布局
void WindowFMSign::showView()
{
	//每日签到左边标签
	auto leftOfSign = Sprite::createWithSpriteFrameName(GOLD_LEFT);
	leftOfSign->setPosition(267, pMainNode->getContentSize().height * 0.47);
	pMainNode->addChild(leftOfSign);

	//左边标签上的数字
	m_labelContinue = LabelTTF::create("0", "Arial", 28);
	m_labelContinue->setPosition(leftOfSign->getContentSize().width * 0.59, leftOfSign->getContentSize().height * 0.78);
	m_labelContinue->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	leftOfSign->addChild(m_labelContinue);

	//左边标签上的蓝色宝箱
	m_continueSignBtn = createButton_SHZ(GOLD_BOX3, nullptr, nullptr, this, cccontrol_selector(WindowFMSign::onBtnClicked));
	m_continueSignBtn->setPosition(leftOfSign->getContentSize().width * 0.5, leftOfSign->getContentSize().height * 0.58);
	m_continueSignBtn->setTag(CONTINUE_SIGN_BOX_TAG);
	leftOfSign->addChild(m_continueSignBtn);
	m_continueSignBtn->setEnabled(false);

	//每日签到右边面板
	auto rightOfSign = Sprite::createWithSpriteFrameName(GOLD_RIGHT);
	rightOfSign->setPosition(Vec2(609,349));
	pMainNode->addChild(rightOfSign);

	//右边面板上的scrollView
	m_scrollView = ui::ScrollView::create();
	m_scrollView->setContentSize(Size(SCROLL_VIEW_WIDTH, SCROLL_VIEW_HEIGHT));
	m_scrollView->setPosition(Vec2(0, 3));
	m_scrollView->setBounceEnabled(true);
	rightOfSign->addChild(m_scrollView);

	m_scrollView->setInnerContainerSize(Size(SCROLL_VIEW_WIDTH, SCROLL_VIEW_LINE_HEIGHT * 6));

	for (size_t row = 0; row < SIGN_ROW; row++)
	{
		for (size_t col = 0; col < SIGN_COL; col++)
		{
			Sprite *dailySignBox = nullptr;
			ControlButton *dailySignButton = nullptr;
 			if (col == 4)
 			{
 				dailySignBox = Sprite::createWithSpriteFrameName(GOLD_SQUARE2);
 
 				if (row == 5)
 				{//最后一行最后一个宝箱
					dailySignButton = createButton_SHZ(GOLD_BOX2, nullptr, nullptr, this, cccontrol_selector(WindowFMSign::onBtnClicked));
 				}
 				else
 				{//最后一列为宝箱
					dailySignButton = createButton_SHZ(GOLD_BOX1, nullptr, nullptr, this, cccontrol_selector(WindowFMSign::onBtnClicked));
 				}
 			}
 			else
 			{//普通金币
 				dailySignBox = Sprite::createWithSpriteFrameName(GOLD_SQUARE1);
				dailySignButton = createButton_SHZ(GOLD_BOX0, nullptr, nullptr, this, cccontrol_selector(WindowFMSign::onBtnClicked));
 			}
			dailySignBox->setPosition(56 + 105 * col, SCROLL_VIEW_LINE_HEIGHT * 0.5 + SCROLL_VIEW_LINE_HEIGHT * (5 - row));
			dailySignBox->setScaleY(1.15);
			m_scrollView->addChild(dailySignBox);
			dailySignButton->setTag(SIGN_DAY_TAG_BASE + row * 5 + col);
			dailySignButton->setPosition(Vec2(dailySignBox->getPosition().x, dailySignBox->getPosition().y-6));
			dailySignButton->setEnabled(false);
			m_scrollView->addChild(dailySignButton);

			LabelAtlas* goldLabel = LabelAtlas::create("", GOLD_NUMBER, 13, 22,'.');
			goldLabel->setPosition(Vec2(dailySignButton->getContentSize().width * 0.62, 0));
			goldLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			dailySignButton->addChild(goldLabel);			
			goldLabel->setScaleX(0.75);
			goldLabel->setScaleY(0.85);

			//设置每一行每一列的金币数值
			goldLabel->setString(StringUtils::format("%d", goldArray[row][col]));

			auto signGold = Sprite::createWithSpriteFrameName(GOLD_TEXT);
			signGold->setPosition(Vec2(dailySignButton->getContentSize().width * 0.86, 0));
			dailySignButton->addChild(signGold);
			signGold->setScale(0.85);

			auto signVip = Sprite::createWithSpriteFrameName(GOLD_VIP);
			signVip->setPosition(signVip->getContentSize().width * 0.5, dailySignBox->getContentSize().height - signVip->getContentSize().height * 0.5-2);
			dailySignBox->addChild(signVip);
		}
	}
	
}

//按钮事件
void WindowFMSign::onBtnClicked(Ref* pSender, Control::EventType type)
{
	auto button = dynamic_cast<ControlButton *>(pSender);
	auto tag = button->getTag();
	
	 if (tag == CONTINUE_SIGN_BOX_TAG)
	{
		 LogicFreeMoney* pLogic = LogicFreeMoney::getInstance();
		 pLogic->requestDailySin(2);
		setContinueSignButtonEnabled(false);
	}
	else
	{
		auto block = dynamic_cast<Sprite *>(button->getChildByTag(BLOCK_TAG));
		if (block)
		{
			LogicFreeMoney* pLogic = LogicFreeMoney::getInstance();
			pLogic->requestDailySin(1);

			block->stopActionByTag(SIGN_DAY_BLOCK_TAG);
			block->removeFromParent();
		}
		button->setEnabled(false);
	}
}

//控制蓝色宝箱是否可以签到
void WindowFMSign::setContinueSignButtonEnabled(bool enabled)
{
	m_continueSignBtn->setEnabled(enabled);
	if (enabled)
	{
		auto rotateBy1 = RotateBy::create(0.1, -20);
		auto rotateBy2 = RotateBy::create(0.1, 20);
		auto seq = Sequence::create(rotateBy1, rotateBy2, rotateBy2->clone(), rotateBy1->clone(), DelayTime::create(1.0f), nullptr);
		auto repeat = RepeatForever::create(seq);
		repeat->setTag(CONTINUE_SIGN_BOX_ACTION_TAG);
		m_continueSignBtn->runAction(repeat);
	}
	else
	{
		m_continueSignBtn->stopActionByTag(CONTINUE_SIGN_BOX_ACTION_TAG);
		m_continueSignBtn->setRotation(0);
	}
}


//获取签到按钮的位置
ControlButton *WindowFMSign::getDailySignButton(int tag)
{
	auto button = dynamic_cast<ControlButton *>(m_scrollView->getChildByTag(tag));
	return button;
}

//提示签到的旋转
void WindowFMSign::addBlockAnimation(int tag)
{
	auto button = getDailySignButton(tag);
	if (!button) return;

	auto block = Sprite::createWithSpriteFrameName("block_0.png");
	auto animate = createAnimate("block_%d.png", 0, 5, 0.6 / 5);
	auto repeat = RepeatForever::create(animate);
	repeat->setTag(SIGN_DAY_BLOCK_TAG);
	block->runAction(repeat);
	block->setPosition(button->getPreferredSize().width * 0.5, button->getPreferredSize().height * 0.5+6);
	block->setTag(BLOCK_TAG);
	button->addChild(block, 10);
}
//创建提示签到的动画旋转
Animate *WindowFMSign::createAnimate(const char *nameFormat, int startIndex, int endIndex, float delay)
{
	Vector<SpriteFrame*> animFrames;
	char szFrameName[128] = { 0 };
	for (int i = startIndex; i < endIndex; i++)
	{
		sprintf(szFrameName, nameFormat, i);
		SpriteFrame* frameItem = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFrameName);
		animFrames.pushBack(frameItem);
	}

	return Animate::create(Animation::createWithSpriteFrames(animFrames, delay));
}

//添加已领取标签
void WindowFMSign::addGotSign(int tag)
{
	auto button = getDailySignButton(tag);
	if (!button) return;
	auto got = Sprite::createWithSpriteFrameName(GOLD_GET);
	got->setPosition(button->getPreferredSize().width * 0.5, button->getPreferredSize().height * 0.5);
	button->addChild(got, 10);
}

//设置签到按钮可触摸
void WindowFMSign::setDailySignButtonEnabled(int tag, bool enabled)
{
	auto button = getDailySignButton(tag);
	if (!button) return;
	button->setEnabled(enabled);
}

void WindowFMSign::updateWindow(int nCode)
{
	if (2 == nCode)
	{
		onQueryDailySign();
	}
}
