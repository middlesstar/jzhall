#ifndef SHZ__Views_H_
#define SHZ__Views_H_

#include "shuihu/View/SpriteHelper_SHZ.h"
#include "shuihu/View/Dialog/Dialog_SHZ.h"
#include "Sound/SoundManager.h"
#include "shuihu/gPlatform_SHZ.h"
#include "shuihu/Game/CMD_CatchBird_SHZ.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UILayout.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

//打开连接
void open_url_SHZ(const char* url);
//通用对话框
void popup_SHZ(const char* title, const char* content, int iButtonType = 0, float delayShow = 0, cocos2d::CCObject* pTarget = 0, cocos2d::SEL_CallFuncN func = 0, cocos2d::CCNode* parent = 0);

// 格式化货币
std::string FormatMoneyFrom_SHZ(const std::string& sIn);

// 格式化货币
template<typename T>
std::string FormatMoney_SHZ(const T& val)
{
	std::ostringstream ost;
	ost.str("");
	ost << val;
	return FormatMoneyFrom_SHZ(ost.str());
}

const char* FormatTime_SHZ(int time);

ControlButton* createButton_SHZ(const char* normal, const char* selected, const char* disable, CCObject* target, cocos2d::extension::Control::Handler handle, bool isZoomOnTouchDown = true);

// 裁剪框  
class SHZTailorView : public Layout
{
public:

	static SHZTailorView* create(const CCSize &size)
	{
		SHZTailorView* pLayer = new SHZTailorView();
		pLayer->setAnchorPoint(Vec2(0, 0));
		pLayer->setContentSize(size);
		pLayer->setPosition(Vec2(0,0));
		pLayer->setClippingEnabled(true);
		pLayer->autorelease();
		return pLayer;

	}

private:
};


#endif // _Views_H_