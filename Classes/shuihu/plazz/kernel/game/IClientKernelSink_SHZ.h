#ifndef SHZ_IClientKernelSink_H_
#define SHZ_IClientKernelSink_H_

#include "shuihu/plazz/kernel/user/IClientUserItem_SHZ.h"

class SHZIClientKernelSink
{
public:
	virtual ~SHZIClientKernelSink(){};

	//控制接口
public:
	//启动游戏
	virtual bool SetupGameClient()=0;
	//重置游戏
	virtual void ResetGameClient()=0;
	//关闭游戏
	virtual void CloseGameClient()=0;

	//框架事件
public:
	//系统滚动消息
	virtual bool OnGFTableMessage(const char* szMessage)=0;
	//等待提示
	virtual bool OnGFWaitTips(bool bWait)=0;
	//比赛信息
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo)=0;
	//比赛等待提示
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)=0;
	//比赛结果
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult)=0;

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(void* data, int dataSize)=0;
	//场景消息
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)=0;
	//场景消息
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize)=0;

	//用户事件
public:
	//用户进入
	virtual void OnEventUserEnter(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户离开
	virtual void OnEventUserLeave(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户积分
	virtual void OnEventUserScore(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户状态
	virtual void OnEventUserStatus(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户属性
	virtual void OnEventUserAttrib(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//用户头像
	virtual void OnEventCustomFace(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
};

#endif // _IClientKernelSink_H_