#include "UIComment_SHZ.h"
#include "df/DF.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "df/LocalStore.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define COMMENT_PLIST_SHZ    "comment/comment.plist"
#define COMMENT_TEXTURE_SHZ  "comment/comment.png"

#define NETWORK_COMMENT_BG_SHZ "gold_bg.png"
#define LOCAL_COMMENT_BG_SHZ   "silver_bg.png"

#define IMG_GC_BTN_SHZ  "gc_btn.png"
#define IMG_NT_BTN_SHZ  "nt_btn.png"

#define TAG_GC_SHZ  1
#define TAG_NT_SHZ  2

SHZUIComment::SHZUIComment()
{

}

SHZUIComment::~SHZUIComment()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(COMMENT_PLIST_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(COMMENT_TEXTURE_SHZ);
}

bool SHZUIComment::init()
{

	do
	{

		CC_BREAK_IF(!CCLayer::init());
		CC_BREAK_IF(!this->setUpView());
		return true;

	} while (0);

	return false;
}

bool SHZUIComment::setUpView()
{

	mShieldLayer = SHZShieldLayer::create(-133);
	addChild(mShieldLayer);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(COMMENT_PLIST_SHZ);

	CCSprite* bgSprite;
	if (0 == DF::shared()->GetGameType())
		bgSprite = CCSprite::createWithSpriteFrameName(NETWORK_COMMENT_BG_SHZ);
	else
		bgSprite = CCSprite::createWithSpriteFrameName(LOCAL_COMMENT_BG_SHZ);
	bgSprite->setPosition(Vec2(0, 0));
	this->addChild(bgSprite);

	float bfW = bgSprite->getContentSize().width;
	float bfH = bgSprite->getContentSize().height;

	ControlButton* Gc_btn = createButton_SHZ(IMG_GC_BTN_SHZ, 0, 0, this, cccontrol_selector(SHZUIComment::btnClick));
	Gc_btn->setPosition(Vec2(bfW*0.25, bfH*0.3));
	//Gc_btn->setTouchPriority(-134);
	Gc_btn->setTag(TAG_GC_SHZ);
	bgSprite->addChild(Gc_btn);

	ControlButton* Nt_btn = createButton_SHZ(IMG_NT_BTN_SHZ, 0, 0, this, cccontrol_selector(SHZUIComment::btnClick));
	Nt_btn->setPosition(Vec2(bfW*0.75, bfH*0.3));
	Nt_btn->setTag(TAG_NT_SHZ);
	//Nt_btn->setTouchPriority(-134);
	bgSprite->addChild(Nt_btn);

	return true;
}

void SHZUIComment::btnClick(CCObject* obj, Control::EventType e)
{
	ControlButton* btn = (ControlButton*)obj;

	switch (btn->getTag())
	{
	case TAG_GC_SHZ:

		//ÍøÂç°æ
		if (DF::shared()->GetGameType() == 0)
		{
		   CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_NETWORK_COMMENT, 1);

		}
		
		break;
	case TAG_NT_SHZ:
		this->removeFromParentAndCleanup(true);
		break;
	default:
		break;
	}
}