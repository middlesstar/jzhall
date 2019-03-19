#pragma once

#include "slwh/ResourcePath.h"

/*
	玩家列表单元
*/


class UserListOne 
{
protected:
	ImageView*			m_rootNode;								//根节点(ImageVirw类型用于ListView占位)
	ImageView*			m_head;									//头像
	ImageView*			m_headBk;								//头像边框
	ImageView*			m_vip0;									//vip0
	ImageView*			m_vip;									//vip金色
	TextAtlas*			m_vipNum;								//vip数字
	Text*				m_name;									//昵称
	ImageView*			m_winBk;								//赢输分数文本背景
	Text*				m_winNum;								//赢输分数文本（单位K）
	TextAtlas*			m_scoreNum;								//分数数字
	

	static UserListOne*	m_templateThis;							//模板


	PlayerInfo			m_userInfo;								//玩家信息

protected:
	UserListOne();
	
	void setWinScore(SCORE score, int type);					//type = 0 压分阶段 type = 1输赢结算
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//头像背景框与VIP相关
public:
	~UserListOne();
	PlayerInfo getuserinfo()const { return m_userInfo; }
	void setuserinfo(const PlayerInfo & info);
	
	ImageView* getRootNdoe(){ return m_rootNode; }

	


	static void initTemplate(ImageView* node);					//初始化模板才能创造对象(模板在cocos studio已拼好)
	static UserListOne* create();
};



