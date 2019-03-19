#pragma once

#include "slwh/Base/UIBase.h"





class JetAndResManager :
	public UIBase
{
protected:
	//enum enumState
	//{
	//	Up = 20,
	//	Down = 0
	//};

	enum TimeCountTitle
	{
		enumInvalideta = -1,

		enumPleaseJet,
		enumCountJet,
		enumGameScene,
		enumJetScene,
		enumOpen,								//正在开奖

		enumMax
	};



	//bool m_bIsRun;										//动画互斥
	bool m_bIsOpen;										//是否已经打开面板
	bool m_bIsEnble;									//是否可用（弹出面板）
	//动画属性
	//float m_speed;
	//int m_index;										//当前动画帧
	//enumState m_targetIndex;							//目标位置

	int	m_nowTime;										//压分剩余时间
	int m_countjettime;									//统计分数3秒
	TimeCountTitle m_nowTitle;							//当前标题
protected:
	Node*			m_TimeCountNode;					//计时器节点
	Button*			m_jetimg;
	Node*			m_moveNode;							//移动节点
	ImageView*		m_title;							//下边小标题
	TextAtlas*		m_timeCount;						//时间倒计时
	Sprite*			m_dot;								//省略号
	ActionInterval*	m_dotAc;							//省略号动画
public:
	JetAndResManager();
	~JetAndResManager();
	//0正常 1强制打开 2强制缩回
	void MyJettonOut(int code = 0);						
	//关联开奖、用户列表、压分让其跟随
	void Attach3Nodes(Node* Open, Node* Jet, Node* Userlist);
	//按钮响应事件
	void BtEvent(Ref* pSender, TouchEventType type);
	//是否可见
	void setVisible(bool b);							//是否隐藏
	//开启倒计时
	void StartPlay(int time);
	//得到当前剩余时间
	int GetleftTime(){ return m_nowTime; }
	//初始化面板的位置  不要轻易改这个初始化位置  会出现蛋疼BUG
	void initposition();
	//防止图片消失,在推出时调用
	void inhideImg();
protected:
	virtual void onEnter() override;

	virtual void onExit() override;

	//设置时间UI
	void setTimeUI(int t);

	void Start();
	//动画过程
	void Running(float t);

	bool GetIsOpen();
	//时间和点的计时器
	void updateDot_Time(float t);
	//2个位置的切换
	void moveTimeCountTojet(bool b);					//是否移动节点到压分界面
	//设置标题
	void setTitle(TimeCountTitle e);
	//打开或面板
	void show(bool b);

};


