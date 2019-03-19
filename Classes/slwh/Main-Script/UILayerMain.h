#ifndef __UILayerMain_H__
#define __UILayerMain_H__

#include "cocos2d.h"
#include "UI/MyJetton/MyJettonManager.h"
#include "UI/UIUserInfo.h"
#include "UI/TimeCount.h"
#include "UI/ResRecord.h"
#include "UI/JCNum.h"
#include "UI/UIMenu.h"
#include "UI/Regular.h"
#include "UI/ZHXControl.h"
#include "UI/Result/ResultManager.h"
#include "slwh/Main-Script/UI/UserList/UserListManager.h"
#include "slwh/Main-Script/UI/WaitNextJet.h"
#include "slwh/Main-Script/UI/JetAndResManager.h"
USING_NS_CC;


#ifndef _ZERO
#define _ZERO 0
#endif // !_ZERO


class UILayerMain : public cocos2d::Layer
{
protected:
	Node*					m_rootNode;					//UI根节点
	Node*                   Jetroot;                    
	Node*                   listroot;
	Node*                   openroot;
	Node*                   jetbt;
	bool					m_bEnble;					//是否可以押注

public:
	MyJettonManager			m_myJetManager;				//押注界面管理者
	UserListManager			m_userListManager;			//用户列表管理者
	ResultManager			m_resManager;				//结果面板控制
	JetAndResManager		m_jetandresManager;			//压分和结果管理(包含一个按钮)

	UIUserInfo				m_uiUserInfo;				//游戏界面的分数
	WaitNextJet				m_waitNextJet;				//等待下一次开奖
	ResRecord				m_resRecord;				//记录板
	JCNum					m_jcNum;					//奖池金币数
	UIMenu					m_uiMenu;					//菜单
	ZHXControl				m_zhxControl;				//ZHX控制
	
public:
	//virtual void onEnter() override;

	//virtual void onExit() override;

	virtual bool init();
	UILayerMain();
	~UILayerMain();
	//屏幕触摸响应事件
	bool onTouchBegan(Touch* touch, Event* event);
	//按钮响应事件
	//void OutBetEvent(Ref *pSender, TouchEventType type);
	void SetEnble(bool b);

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;

	CREATE_FUNC(UILayerMain);


};

#endif

