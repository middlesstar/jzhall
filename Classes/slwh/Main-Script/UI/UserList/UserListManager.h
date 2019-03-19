#pragma once
#include "UserListOne.h"
#include "slwh/Base/UIBase.h"

/*
一个压分按钮的脚本
*/


class UserListManager 
	: public UIBase
{
protected:
	Sprite*							m_head;
	ImageView*						m_headBk;
	Text*							m_name;
	ImageView*						m_vip0;
	ImageView*						m_vip;
	TextAtlas*						m_vipNum;
	TextAtlas*						m_scoreNum;
	Text*							m_onlineNum;						//在线人数
	Text*							m_totelJetNum;						//总压分
	ImageView*						m_winBk;							//赢输分数文本背景
	Text*							m_winNum;							//赢输分数文本（单位K）
	Sprite*							m_betgold;							//金币图片

	ListView*						m_userListView;
	std::vector<UserListOne*>		m_userlistinfo;

	float							m_actiontime;						//动画时间
	SCORE							m_getnum;							//得到的金币
	SCORE							m_targetGold;						//滚动到目标金币
protected:
	//初始化
	void Start();
	void setOnlineNum(int i);									//设置在线玩家 
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//头像背景框与VIP相关

	//想促发朕金币滚动的 调用此函数
	void goldRollPlay(SCORE targetnum);
	//进步滚动开始动画 与 金币喷泉配套 
	void goldRolldelay(float  t);
	void goldRollfunc(float  t);
public:
	//刷新界面显示
	void refreshPlayerCoin(Ref* ref);
	//刷新界面vip显示
	void refreshPlayerVIP(Ref* ref);
public:
	//牵线搭桥 贡献出m_betgold指针
	//Sprite* GetBetGold(){ return m_betgold; }
	void setTotelNum(SCORE score);								//设置总压分
	//添加玩家结果
	void addUserElement(const slwhPlayerInfo &info);
	//更新玩家列表
	void updateUserList(std::vector<slwhPlayerInfo> vec);
	//初始化玩家列表
	void InitUserList();
	//删除玩家
	void deleteUserElement(int userid);
	//有玩家压分
	void otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex);
	//有玩家取消压分
	void othercancelJet(int userid, SCORE jetadd);
	//更新自己信息    //需求要加金币滚动效果，实在是蛋疼，不想改变原有的结构，在此函数加个参数 以此作为标记在需要的位置滚动
	void updateInfo(int code = 0);
	//设置输赢分数和压分
	void setWinScore(SCORE score, bool biscount);

	UserListManager();

	~UserListManager();
	//测试
	void update(float t);
};



	