#ifndef __MyJettonManager_H__
#define __MyJettonManager_H__

#include "OneOfJetton.h"
#include "slwh/Module/SLWHGoldHeap.h"

/*
压分面板管理类
*/

#ifndef Jetton_Btn_Max
#define Jetton_Btn_Max	15
#endif

#define RECORDMAX 12

class MyJettonManager :
	public UIBase
{
protected:

	//需求的改动 与主游戏场景结果记录重复的逻辑  直接复制过来的， 这块没有设计好
	struct Record
	{
		ImageView* animal;
		ImageView* zhx;
	};
	std::map<int, std::string> m_map;

	int m_listRear;									//循环队列尾
	Record m_jetrecord[RECORDMAX];
	//Vec2 m_ptEnd;									//终点坐标
protected:
	//Jetton_Btn_Max个压分面板按钮  顺序： (红)动物1 ~ 4   (绿)~    (黄)~    Z H X
	OneOfJetton		m_jetArray[Jetton_Btn_Max];
	//是否开启了 AI
	bool			m_bIsAI;
	//上一次有效压分
	int				m_lastValidRecode[Jetton_Btn_Max];
	//上一次有效压分总数
	int				m_lastValidAll;
	//上一次压分记录 
	int				m_lastRecode[Jetton_Btn_Max];
	//上一次压分总数
	int				m_lastGameAll;
	//是否可用
	bool			m_bIsenable;
	//按钮总压分列表 金币堆
	SLWH_GoldHeap*	m_oneofjet[Jetton_Btn_Max];
protected:
	void SetZHXTex(int index, enumGameZXH type);
	void Start();
	void ContinuJetEvent(Ref *pSender, TouchEventType type);
	void CancelJetEvent(Ref *pSender, TouchEventType type);
	void AIEvent(Ref *pSender, TouchEventType type);
	void AICancelEvent(Ref *pSender, TouchEventType type);

public:
	MyJettonManager();
	~MyJettonManager();

public:
	void SaveLastGame();
	void SetRate(int* a);
	void ClearJetton();
	void SetAllJetton(int* g);
	void SetAI(bool b);
	int* GetRecord();
	bool GetAI();
	int  GetLastJetAll();
	bool ContinuJet();
	bool CancelJet();
	//初始化记录信息接口  个位数为庄闲和  
	void InitResList(int* a);
	//*****************************************************************************************
protected:

	Node*		m_moveNode;
	//押注按钮（即m_node本身）
	//Button*		m_Jet;
	//压分选择按钮
	Button*		m_betAdd;
	//当前压分档
	TextAtlas*	m_betText;
	//自动按钮
	Button*		m_ai;
	//续压按钮
	Button*		m_continu;
	//取消压分按钮
	Button*		m_cancel;
	//取消自动
	Button*		m_cancelAI;
	//颜色层
	LayerColor* m_layerColor;
	//总压分 
	TextAtlas*	m_totleJet;
	//彩金
	TextAtlas*	m_caijin;
	//记录面板
	Node*		m_record;
protected:
	//设置我的总压分
	void setMyTotelJet(SCORE score);
	//增加压分档事件
	void JetAddEvent(Ref *pSender, TouchEventType type);
	
public:
	//这是压分档
	void SetJetNum(int i);

	void update(float t);

	bool onTouchBegan(Touch* touch, Event* event);
	//是否能压分
	void enable(bool b);
	//显示
	virtual void StartPlay();
	//隐藏
	virtual void StopPlay();
	//更新金币堆定时器（押注阶段更新）
	void updategoldheap(float t);
	//设置彩金
	void setCaijin(SCORE score);

	//添加到结果节点
	void Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4);

	void Add(AnimalResult e);
	//开始游戏召唤开奖界面
	void OpenRecord();
	//请求压分成功
	void successJet(SCORE gold, SCORE score, dword animalID);
	//请求取消压分成功
	void successCancelJet();
};

#endif

