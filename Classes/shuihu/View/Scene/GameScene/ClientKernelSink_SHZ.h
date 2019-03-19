#ifndef SHZ__ClientKernelSink_H_
#define SHZ__ClientKernelSink_H_

#include "cocos2d.h"
#include "GameClientView_SHZ.h"

class SHZClientKernelSink
	: public SHZIClientKernelSink
{

	//控制接口
public:

	//启动游戏
	virtual bool SetupGameClient();
	//重置游戏
	virtual void ResetGameClient();
	//关闭游戏
	virtual void CloseGameClient();

	//框架事件
public:
	//系统滚动消息
	virtual bool OnGFTableMessage(const char* szMessage);
	//等待提示
	virtual bool OnGFWaitTips(bool bWait);
	//比赛信息++
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo);
	//比赛等待提示
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip);
	//比赛结果
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult);

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//用户离开
	virtual void OnEventUserLeave(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//用户积分
	virtual void OnEventUserScore(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//用户状态
	virtual void OnEventUserStatus(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//用户属性
	virtual void OnEventUserAttrib(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//用户头像
	virtual void OnEventCustomFace(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};


	//////////////////////////////////////////////////////////////////////////

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(void* data, int dataSize);
	//场景消息
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	//游戏消息
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize);

	//消息处理
protected:
	//开始出牌
	virtual bool  StartGetType();
	//获取得分
	virtual bool  GetWinScore(int gettype);
	//上下分
	virtual bool AddOrRemoveScore(bool addorremove);
	//增加上注
	virtual bool  ChangeCellScore();
	//
	virtual bool MaLiSendMessage();

	//获取比倍数据
	virtual bool  GetBiBeiData();





	//切换座位
	int SwitchChairID(int m_Currchairid);
	
	//////////////////////////////////////////////////////////////////////////
	// 功能函数
public:
	void send_time_check();

	
	//////////////////////////////////////////////////////////////////////////
	

private:
	
	bool					mIsNetworkPrepared;
	SHZCGameClientView*		m_GameClientView;					//游戏视图

};

extern SHZClientKernelSink gClientKernelSink_SHZ;
#endif // _ClientKernelSink_H_