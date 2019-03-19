#ifndef __OneOfJetton_H__
#define __OneOfJetton_H__

#include "slwh/Base/UIBase.h"


/*
一个压分按钮的脚本
*/



class OneOfJetton : public UIBase
{
protected:
	static Vec2			m_sTarPos[15];										//金币飞行15个目标位置
	int					m_jet;												//当前押注数额
	TextAtlas*			m_beilvNum;											//倍率艺术数字
	TextAtlas*			m_myJetNum;											//我的押注艺术数字
	int					m_tag;												//按照索引是第几个按钮		12=庄 13=和 14=闲			

	
	static int			JetAdd;												//压分增加
public:
	static SCORE		MyAllTotel;											//我的总压分
    bool			BIsCanJet;										//是否可以压分	发送数据包，成功接收便可继续压分			
public:
	OneOfJetton();
	~OneOfJetton();

	//按钮响应事件
	void BtEvent(Ref *pSender, TouchEventType type);

protected:
	//播放星星粒子特效
	void PopStarPlay();

	void GoldFlyStartPlay();

	void Start();

public:
	//设置压分档 0 = 默认 ,1 = 初始化
	static int SetAddJet(int code = 0); 
	//设置按钮索引
	void SetTag(int x){ m_tag = x; }
	//得到按钮索引
	int GetTag(){ return m_tag; }
	//设置倍率
	void SetBeiLv(int g);
	//设置我的压分
	void SetMyJetton(SCORE g);
	//设置总压分
	//void SetAllJetton(int g);
	//增加压分
	void AddJet();
	//服务器返回压分成功 刷新压分
	void UpdateJet(SCORE score);
	//得到当前压分
	int GetJet(){ return m_jet; }
	
};

#endif

