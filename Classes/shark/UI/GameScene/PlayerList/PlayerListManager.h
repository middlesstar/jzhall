#ifndef _PlayerListManager_H_
#define _PlayerListManager_H_

#include "StakeItem.h"
#include "PlayerListOne.h"
#include "shark/Base/ViewBase.h"

/*
一个压分按钮的脚本
*/


class PlayerListManager 
	: public ViewBase
{
protected:
	Node*                           playerInfo;
	Node*                           stakeInfo;
	Sprite*							m_head;
	ImageView*						m_headBk;
	Text*							m_name;
	ImageView*						m_vip0;
	ImageView*						m_vip;
	TextAtlas*						m_vipNum;
	Text*							m_scoreNum;
	Text*							m_onlineNum;						//在线人数
	Text*							m_totelJetNum;						//总压分
	ImageView*						m_winBk;							//赢输分数文本背景
	Text*							m_winNum;							//赢输分数文本（单位K）
	Sprite*							m_betgold;							//金币图片

	ListView*						m_playerListView;
	std::vector<PlayerListOne*>		m_playerlistinfo;
	//押分信息部分
	Text*					     	totalStake;
	ListView*                       stakeListView;
	std::vector<StakeItem*>         stakeListInfo;

	SCORE							m_getnum;							//得到的金币
	SCORE							m_targetGold;						//滚动到目标金币
	int                             countStake;                         //记录有几个押分按钮
protected:
	//初始化
	void setContent();
	void setOnlineNum(int i);									//设置在线玩家 
	void setTotelNum(SCORE score);								//设置总压分
	void setScore(SCORE score);
	void setName(const std::string &name);
	void setHead(int index);
	void setVipOrHradBk(int vip);								//头像背景框与VIP相关

	//设置总押分数
	void setTotalStake(int num);


	//促发金币滚动的 调用此函数
	void goldRollPlay(SCORE targetnum);
	//进步滚动开始动画 与 金币喷泉配套 
	void goldRolldelay(float  t);
	void goldRollfunc(float  t);


public:
	//添加玩家结果
	void addPlayerElement(const UserInfo_Shark &info);
	//更新玩家列表
	void updatePlayerList(std::vector<UserInfo_Shark> vec);
	//初始化玩家列表
	void InitPlayerList();
	//删除玩家
	void deleteUserElement(int userid);
	//有玩家压分
	void otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex);
	//有玩家取消压分
	void othercancelJet(int userid, SCORE jetadd);
	//更新自己信息
	void updateInfo(int code = 0);

	//设置输赢分数和压分
	void setWinScore(SCORE score, gameState type);

	void cancelTotleNum();                                //清空上一局的所有人押分总信息
	
	PlayerListManager();

	~PlayerListManager();
public:
	//刷新界面显示
	void refreshPlayerCoin(Ref* ref);
	//刷新界面vip显示
	void refreshPlayerVIP(Ref* ref);
public:
	//显示押分信息
	void showStakeInfo();

	//删除自己的押分列表信息
	void deleteOwnStake();

public:
	//转换列表视图 
	void changeView(int index);            //1.玩家列表2.押注列表


};


#endif 