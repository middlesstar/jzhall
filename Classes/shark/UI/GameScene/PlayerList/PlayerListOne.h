#ifndef _PlayerListOne_H_
#define _PlayerListOne_H_

#include "shark/resourceDef.h"
#include "shark/Base/ViewBase.h"
/*
	玩家列表单元
*/

#define _Gold3_Fly		"slwh/animation/gold3/gold3.png"

class PlayerListOne : 
	public Widget
{
protected:
	ImageView*			m_rootNode;								//根节点(ImageView类型用于ListView占位)
	ImageView*			m_head;									//头像
	ImageView*			m_headBk;								//头像边框
	ImageView*			m_vip0;									//vip0
	ImageView*			m_vip;									//vip金色
	TextAtlas*			m_vipNum;								//vip数字
	Text*				m_name;									//昵称
	ImageView*			m_winBk;								//赢输分数文本背景
	Text*				m_winNum;								//赢输分数文本（单位K）
	TextAtlas*			m_scoreNum;								//分数数字

	
	static PlayerListOne*	m_templateThis;							//模板

	UserInfo_Shark			m_userInfo;								//玩家信息

	static Vec2		    	m_targetPos[12];							//金币飞行15个目标位置

	
protected:
	PlayerListOne();
	
	void setWinScore(SCORE score, int type);					//type = 0 压分阶段 type = 1输赢结算
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//头像背景框与VIP相关
	
	//回调销毁
	void CallBack(Node* node);

public:
	~PlayerListOne();
	UserInfo_Shark getuserinfo()const { return m_userInfo; }
	void setuserinfo(const UserInfo_Shark & info);
	
	ImageView* getRootNdoe(){ return m_rootNode; }
	void GoldFly(int tag);                                      //飞金币效果
	static void initTemplate(ImageView* node);					//初始化模板才能创造对象(模板在cocos studio已拼好)
	static PlayerListOne* create();
};


#endif 
