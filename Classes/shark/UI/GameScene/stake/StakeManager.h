#ifndef _StakeManager_H_
#define _StakeManager_H_

#include "cocos2d.h"
#include "OneOfStake.h"
#include "shark/Module/SharkGoldHeap.h"


#define  AUTOBTN          "btn_auto"
#define  CANCELAUTO       "btn_cancelAuto"
#define  CONTINUEBTN      "btn_continue"
#define  CANCELBTN        "btn_cancel"               //取消押分
#define  STAKEBTN         "btn_subsection"           //押分按钮
#define  STALELABEL       "stakeLabel"               //押分档显示标签
#define  TOTALSCORE       "totalscore"                         //总押分标签
#define  CAIJINSCORE      "winscore"                         //彩金标签

#define  STAKEALL         "topPanel"



#define STAKE_ALL_ARRAY		{"bet_bird","bet_goldShark" , "bet_silverShark", "bet_beast", "bet_swallow", "bet_dove", "bet_peacock", "bet_eagle", "bet_lion", "bet_panda", "bet_monkey", "bet_rabbit" }

#ifndef Stake_Btn_Max
#define Stake_Btn_Max	12
#endif

class StakeManager : public ViewBase
{
public:

	StakeManager();
	~StakeManager();
	CREATE_FUNC(StakeManager);

	virtual void setContent();
	
protected:
	Node*    pMainNode;
	Button*  m_stake;
	Button*  m_cancel;
	Button*  m_continue;
	Button*  m_auto;
	Button*  m_cancelAuto;
	//当前压分档
	TextAtlas*	m_betText;

	//颜色层
	LayerColor*    m_shieldLayer;
	//是否可用
	bool			m_bIsenable;

	//按钮总压分列表 金币堆
	SharkGoldHeap*	m_oneofjet[Stake_Btn_Max];

public:
	void SaveLastGame();
	void SetAllStake(int* g);
	int* GetRecord();
	//设置倍率
	void SetRate(int* a);
	void ClearJetton();
	void SetAI(bool b);
	bool GetAI();
	int  GetLastStakeAll();
	bool ContinuStake();
	bool CancelStake();

	//玩家离开时结算列表上的信息
	void playerOut(Ref* ref);

protected:
	
	void ContinuJetEvent(Ref *pSender, TouchEventType type);
	void CancelJetEvent(Ref *pSender, TouchEventType type);
	void AIEvent(Ref *pSender, TouchEventType type);
	void AICancelEvent(Ref *pSender, TouchEventType type);
protected:
	//设置我的总压分
	void setMyTotelStake(SCORE score);
	void SetJetNum(int i);
	void JetAddEvent(Ref *pSender, TouchEventType type);

public:	
	//闪烁按钮上的光圈
	void  showCircle();
	//显示旋转光圈
	void showRing();

	//设置彩金
	void setCaijin(SCORE score);
	//更新我的压分
	void updateMyStake(float t);

	//设置触摸屏蔽
	void setTouch(bool b);

	bool onTouchBegan(Touch* touch, Event* event);

	//请求压分成功
	void successStake(SCORE gold, SCORE score, dword animalID);
	//请求取消压分成功
	void successCancelStake();

	//更新金币堆定时器（押注阶段更新）
	void updateGoldheap(float t);
	//金币堆消除
	void hideGoldheap();
	//开启金币堆定时器
	void startGoldheap();
private:
	cocos2d::EventListenerTouchOneByOne* eventListener;
private:
	cocos2d::EventListenerTouchOneByOne* listener;


protected:
	//Stake_Btn_Max个压分面板按钮  
	OneOfStake		m_stakeArray[Stake_Btn_Max];
	//是否开启了 自动
	bool			m_bIsAI;
	//上一次有效压分
	int				m_lastValidRecode[Stake_Btn_Max];
	//上一次有效压分总数
	int				m_lastValidAll;
	//上一次压分记录 
	int				m_lastRecode[Stake_Btn_Max];
	//上一次压分总数
	int				m_lastGameAll;

	//记录本局的倍率
	//int             m_rate[Stake_Btn_Max];


	//总压分 
	TextAtlas*	m_totleStake;
	//彩金
	TextAtlas*	m_caijin;

};

#endif // _StakeManager_H_
