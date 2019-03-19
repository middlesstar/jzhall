#include "GameHeader_SHZ.h"
#include "ViewHeader_SHZ.h"
#include "shuihu/View/Scene/getHouseIndex_SHZ.h"

SHZDialog* dlg;
void popup_SHZ(const char* title, const char* content, int iButtonType, float delayShow, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncN func, cocos2d::CCNode* parent)
{
	using namespace cocos2d;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCSpriteFrame* frame = cache->spriteFrameByName("fram2.png");
	//auto frame = Sprite::create("");
	auto size = frame->getRect().size;
	auto insetRect = CCRectMake(25, 25, size.width - 25 * 2, size.height - 25 - 25);
	bool flag = SHZGetHouseIndex::getInstance()->getFlag();
	if (!flag)
	{
		// 定义一个弹出层，传入一张背景图
		dlg = SHZDialog::create(frame, insetRect);
		//dlg->setContentSize(CCSizeMake(400, 250));
		dlg->setContentSize(CCSizeMake(533, 347));
		//dlg->setTitle(title);
		dlg->setTitle("");
		dlg->setContentText(content, 25, 25, 24);
		dlg->setCallbackFunc(pTarget, func);
		dlg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

		if ((iButtonType&DLG_MB_OK) || (iButtonType == 0))
		{
			CCSprite* noraml = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_queding_1.png"));
			CCSprite* selected = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_queding_1.png"));
			dlg->addButton(noraml, selected, "", DLG_MB_OK);
		}

		if (iButtonType&DLG_MB_YES)
		{
			CCSprite* noraml = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_queding_1.png"));
			CCSprite* selected = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_queding_1.png"));
			dlg->addButton(noraml, selected, "", DLG_MB_YES);
		}

		if (iButtonType&DLG_MB_NO)
		{
			CCSprite* noraml = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_quxiao_1.png"));
			CCSprite* selected = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_quxiao_1.png"));
			dlg->addButton(noraml, selected, "", DLG_MB_NO);
		}

		if (iButtonType&DLG_MB_CANCEL)
		{
			CCSprite* noraml = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_quxiao_1.png"));
			CCSprite* selected = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("new_dlg_quxiao_1.png"));
			dlg->addButton(noraml, selected, "", DLG_MB_CANCEL);
		}


		dlg->doModal(parent);        
		// 弹出效果
		dlg->setScale(0, 0);
		dlg->setVisible(false);
		CCAction* act = CCSequence::create(
			CCDelayTime::create(delayShow),
			CCShow::create(),
			CCScaleTo::create(0.06f, 1.05f),
			CCScaleTo::create(0.08f, 0.95f),
			CCScaleTo::create(0.08f, 1.0f), 0);
		dlg->runAction(act);
	}
	else
	{
		dlg->remove();
	}	
	
}


void open_url_SHZ(const char* url)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	ShellExecuteA(NULL, NULL, url, NULL, NULL, SW_SHOWNORMAL);
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	//NSString* ns_str = [NSString stringWithUTF8String:url];
	//[[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString:ns_str]]
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#endif
}

// 格式化货币
std::string FormatMoneyFrom_SHZ(const std::string& sIn)
{
	if (sIn.empty())
		return "";

	std::string sRet;
	int iflag = sIn.find_last_of('.');

	std::string s1 = iflag == -1 ? sIn : sIn.substr(0, iflag);
	std::string s2 = iflag == -1 ? "" : sIn.substr(iflag);
	int len = s1.size();

	int iFlag = 0;
	if (s1[0] == '-')
		iFlag = 1;

	int c = 0;

	for (int i = len - 1; i >= iFlag; --i)
	{
		sRet.push_back(s1[i]);
		++c;

		if (c >= 3 && i != iFlag)
		{
			sRet.push_back(',');
			c = 0;
		}
	}

	if (sRet.empty() && !s2.empty())
		sRet.push_back('0');

	if (iFlag == 1)
		sRet.push_back('-');

	//反转
	std::reverse(sRet.begin(), sRet.end());

	//只有一个点
	if (s2.size() == 1)
		s2.clear();

	return sRet + s2;
}


const char* FormatTime_SHZ(int time)
{
	static std::string str;

	int iDay = time / 86400;
	time -= iDay * 86400;
	int iHour = time / 3600;
	time -= iHour * 3600;
	int iMin = time / 60;
	time -= iMin * 60;

	str.resize(128, '\0');
	sprintf(&str[0], "%02d:%02d:%02d", iHour, iMin, time);
	return str.c_str();
}
ControlButton* createButton_SHZ(const char* normal, const char* selected, const char* disable, CCObject* target, cocos2d::extension::Control::Handler handle, bool isZoomOnTouchDown)
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
	bt->setZoomOnTouchDown(isZoomOnTouchDown);
	return bt;
}
