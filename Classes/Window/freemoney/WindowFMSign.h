#ifndef  _WindowFMSign_H_
#define  _WindowFMSign_H_

#include "Window/WindowBase.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Network/CommandDef.h"
#include "Network/CommandStructDef.h"

/* 窗口基类 */
class  WindowFMSign : public WindowBase
{
public:
	WindowFMSign();
	virtual ~WindowFMSign();

	CREATE_FUNC(WindowFMSign);
	virtual void setContent();				                //设置UI布局
	virtual void updateWindow(int nCode);	                //刷新窗口

	virtual void onEnter() override;


//显示界面
public:
	void showView();
	void onQueryDailySign();
private:
	//设置签到按钮可触摸
	void setDailySignButtonEnabled(int tag, bool enabled);
	void onBtnClicked(cocos2d::Ref* pSender, cocos2d::extension::Control::EventType type);
	//控制蓝色宝箱是否可以签到
	void setContinueSignButtonEnabled(bool enabled);
	//提示签到的旋转
	void addBlockAnimation(int tag);
	//获取签到按钮的位置
	cocos2d::extension::ControlButton *getDailySignButton(int tag);
	//创建提示签到的动画旋转
	Animate *createAnimate(const char *nameFormat, int startIndex, int endIndex, float delay);
	//添加已领取标签
	void addGotSign(int tag);

protected:
	Node *pMainNode;
	ui::ScrollView* m_scrollView;
	//显示连续签到的天数
	LabelTTF *m_labelContinue;
	//蓝色宝箱按钮
	cocos2d::extension::ControlButton *m_continueSignBtn;
};

#endif // _WindowFMSign_H_

