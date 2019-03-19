#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "UIShezhi_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "Sound/SoundManager.h"
#include "ShieldLayer_SHZ.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PLIST_PATH_SHZ				"settings/settings.plist"
#define TEXTURE_PATH_SHZ			"settings/settings.png"

#define IMG_SETTING_BG				"settings_bg.png"
#define IMG_SETTING_CLOSE			"settings_bt_close.png"
#define IMG_SETTING_SLIDER_BG		"settings_slider_bar_bg.png"
#define IMG_SETTING_SLIDER_PROGRESS	"settings_slider_bar_progress.png"
#define IMG_SETTING_SLIDER_BTN		"settings_slider_bar_button.png"



static ControlSlider* createSlider_SHZ(const char* bgFile, const char* progressFile, const char* thumbFile)
{
	CCSprite *backgroundSprite = CCSprite::createWithSpriteFrameName(bgFile);
	CCSprite *progressSprite = CCSprite::createWithSpriteFrameName(progressFile);
	CCSprite *thumbSprite = CCSprite::createWithSpriteFrameName(thumbFile);

	ControlSlider* slider = ControlSlider::create(backgroundSprite, progressSprite, thumbSprite);

	return slider;
}

//////////////////////////////////////////////////////////////////////////

SHZUIShezhi::SHZUIShezhi()
{
	mShieldLayer = 0;
}

SHZUIShezhi::~SHZUIShezhi()
{
	SHZSoundManager::shared()->saveSettings();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(PLIST_PATH_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(TEXTURE_PATH_SHZ);
		
}

//初始化方法
bool SHZUIShezhi::init()
{
	do 
	{
	
		CC_BREAK_IF(!CCLayer::init());


		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_PATH_SHZ);

		CCSprite* bg = CCSprite::createWithSpriteFrameName(IMG_SETTING_BG);
		addChild(bg);

		mShieldLayer = SHZShieldLayer::create(-128);
		this->addChild(mShieldLayer);

		//音乐控制按钮
		ControlSlider* slider = createSlider_SHZ(IMG_SETTING_SLIDER_BG, IMG_SETTING_SLIDER_PROGRESS, IMG_SETTING_SLIDER_BTN);
		slider->setAnchorPoint(Vec2(0, 0.5));
		slider->setPosition(Vec2(-163, 63));
		slider->setValue((SoundManager::shared()->getMusicVolume()));
		addChild(slider);

		slider->setMaximumValue(1.0f);
		slider->setMinimumValue(0.0f);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SHZUIShezhi::musicChanged), Control::EventType::VALUE_CHANGED);
	
		//音效控制按钮
		ControlSlider* slider2 = createSlider_SHZ(IMG_SETTING_SLIDER_BG, IMG_SETTING_SLIDER_PROGRESS, IMG_SETTING_SLIDER_BTN);
		slider2->setAnchorPoint(Vec2(0, 0.5));
		slider2->setPosition(Vec2(-163, -15));
		slider2->setValue((SoundManager::shared()->getSoundVolume()));
		addChild(slider2);

		slider2->setMaximumValue(1.0f);
		slider2->setMinimumValue(0.0f);
		slider2->addTargetWithActionForControlEvents(this, cccontrol_selector(SHZUIShezhi::soundChanged), Control::EventType::VALUE_CHANGED);

		//关闭按钮
		ControlButton* btClose = createButton_SHZ(IMG_SETTING_CLOSE, 0, 0, this, cccontrol_selector(SHZUIShezhi::onCloseClick));
		btClose->setTag(200);
		btClose->setPosition(Vec2(0, -200));
		addChild(btClose);

		return true;

	} while (0);

	return false;
}

void SHZUIShezhi::onExit()
{
	CCNode::onExit();

	if (mShieldLayer)
		mShieldLayer->removeFromParentAndCleanup(true);
}

void SHZUIShezhi::musicChanged(Ref* sender, Control::EventType ev)
{
	ControlSlider* slider = (ControlSlider*)sender;
	float value = slider->getValue();
	SoundManager::shared()->setMusicVolume(value);
}

void SHZUIShezhi::soundChanged(Ref* sender, Control::EventType ev)
{
	ControlSlider* slider = (ControlSlider*)sender;
	float value = slider->getValue();
	SoundManager::shared()->setSoundVolume(value);
}

void SHZUIShezhi::onCloseClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	removeFromParentAndCleanup(true);
}