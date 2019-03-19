#include "shuihu/GameHeader_SHZ.h"
#include "UIQuanPan_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "df/LocalStore.h"


USING_NS_CC;
USING_NS_CC_EXT;

#define QUAN_PAN_PLIST      "quanpan/quanpan.plist"
#define QUAN_PAN_TEXTURE    "quanpan/quanpan.png"


#define QUAN_PAN_Network_BG	"qp_local_bg.png"		// network background
#define QUAN_PAN_CLOSE		"x.png"					// close
#define OPEN_BTN_1		    "qp_open_normal.png"	// normal
#define OPEN_BTN_2			"qp_open_normal.png"	// selected
#define OPEN_BTN_3			"qp_open_disabled.png"	// disabled



SHZUIQuanPan::SHZUIQuanPan() : mQuanPanMission(STATIC_DATA_STRING("address"), SERVER_PORT_SHZ), mOpenServer(0)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(QUAN_PAN_PLIST);
	mQuanPanMission.setMissionSink(this);
	memset(mQuanPanTag, 0, sizeof(int)* 8);
}

void SHZUIQuanPan::onEnterTransitionDidFinish()
{
	CCNode::onEnterTransitionDidFinish();
	ReadQuanPanInfo();
}

SHZUIQuanPan::~SHZUIQuanPan()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(QUAN_PAN_PLIST);
	Director::getInstance()->getTextureCache()->removeTextureForKey(QUAN_PAN_TEXTURE);
}

bool SHZUIQuanPan::init()
{

	do{

		CC_BREAK_IF(!CCLayer::init());
		CC_BREAK_IF(!this->SetUpView());

	} while (0);

	return true;
}

bool SHZUIQuanPan::SetUpView()
{
	mShieldLayer = SHZShieldLayer::create(-133);
	addChild(mShieldLayer);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(QUAN_PAN_PLIST);

	CCSprite* __bgSprite;
	if (0 == DF::shared()->GetGameType())
		__bgSprite = CCSprite::createWithSpriteFrameName(QUAN_PAN_Network_BG);
	__bgSprite->setPosition(Vec2(0, 0));
	this->addChild(__bgSprite);


	float __QPos[2][8] = { { 3.0 / 7, 4.0 / 7, 5.0 / 7, 6.0 / 7 - 0.04, 5.0 / 7, 4.0 / 7, 3.0 / 7, 2.0 / 7 + 0.02 },
	{ 0.75, 0.79, 0.75, 0.58, 0.42, 0.4, 0.42, 0.58 }
	};


	char  __Name[12];
	float __bgX = __bgSprite->getContentSize().width;
	float __bgY = __bgSprite->getContentSize().height;

	bool flag = true;
	for (int i = 0; i < 8; i++)
	{

		if (mQuanPanTag[i] == 1)
			sprintf(__Name, "qp_%d1.png", i);
		else {
			flag = false;
			sprintf(__Name, "qp_%d.png", i);
		}

		CCSprite* __Temp = CCSprite::createWithSpriteFrameName(__Name);

		float _fx = __QPos[0][i] - 0.05;
		float _fy = __QPos[1][i];

		__Temp->setPosition(Vec2(_fx * __bgX, _fy*__bgY));

		__bgSprite->addChild(__Temp);
	}

	// 打开全盘button
	ControlButton* btnOpen = createButton_SHZ(OPEN_BTN_1, OPEN_BTN_2, OPEN_BTN_3, this, cccontrol_selector(SHZUIQuanPan::onBtnClick));
	btnOpen->setPosition(Vec2(__bgX / 2, 0.2 * __bgY));
	btnOpen->setTag(200);
	__bgSprite->addChild(btnOpen);
	//btnOpen->setTouchPriority(-134);
	if (!flag) btnOpen->setEnabled(false);

	// 关闭
	ControlButton* btnClose = createButton_SHZ(QUAN_PAN_CLOSE, 0, 0, this, cccontrol_selector(SHZUIQuanPan::onBtnClick));
	btnClose->setPosition(Vec2(__bgX * 0.9, 0.95 * __bgY));
	btnClose->setTag(201);
	__bgSprite->addChild(btnClose);
	//btnClose->setTouchPriority(-134);

	return true;
}

bool SHZUIQuanPan::ReadQuanPanInfo()
{
	int __gType = DF::shared()->GetGameType();
	int __state = 0;

	if (TAG_MODE_LOCAL == __gType)
	{

	}
	else
	{
		mQuanPanMission.getQuanPanStatue();
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////
// ICGPQuanPanMissionSink
void SHZUIQuanPan::onGPQuanPanStatue(uint8 statue)
{
	mStatue = statue;
	ToBin(mStatue, mQuanPanTag);
	log("UIQuanPan::onGPQuanPanStatue %d", mStatue);
	removeAllChildrenWithCleanup(true);
	SetUpView();
}

void SHZUIQuanPan::onBtnClick(CCObject* obj, cocos2d::extension::Control::EventType e)
{
	ControlButton *btn = (ControlButton*)obj;

	switch (btn->getTag())
	{
	case 200:	// 打开

		if (DF::shared()->GetGameType() == 1){
			JZUserDefault::getInstance()->setIntegerForKey(ACCOUNT_SECTION, ACCOUNT_88_STATE, 1);
		}

		if (mOpenServer && mOpenServer->onOpenServerItem(3))
		{
			mQuanPanMission.setQuanPanStatue(0);
			DF::shared()->SetQuanPanState(1);
			btn->setEnabled(false);
		}
		break;
	case 201:	// 关闭
		break;
	default:
		break;
	}
	removeFromParentAndCleanup(true);
}


