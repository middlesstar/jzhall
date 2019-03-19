#ifndef _SharkNetWorkKernel_H_
#define _SharkNetWorkKernel_H

#include "Network/CSocketMission.h"
#include "shark/resourceDef.h"

class SharkNetWorkKernel :
	public CSocketMission
{
protected:

	SharkNetWorkKernel();

	SharkNetWorkKernel(const std::string& url, int port);

	virtual ~SharkNetWorkKernel();

public:
	//释放
	virtual void relese();

	//---------------------网络连接处理处理---------------------//
public:
	//连接上了服务器
	virtual void onEventTCPSocketLink();
	//关闭了服务器
	virtual void onEventTCPSocketShut();
	//服务错误
	virtual void onEventTCPSocketError(int errorCode);
	//网路读取
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

	//---------------------子消息处理---------------------//
protected:
	//登陆命令
	virtual bool onSubCmdLogon(int sub, void* data, int dataSize);
	//配置命令
	virtual bool onSubCmdConfig(int sub, void* data, int dataSize);
	//用户信息
	virtual bool onSunCmdUserInfo(int sub, void* data, int dataSize);
	//游戏命令
	virtual bool onSubCmdGame(int sub, void* data, int dataSize);
	//状态命令
	virtual bool onSubCmdState(int sub, void* data, int dataSize);
	//系统命令 
	virtual bool onSubCmdSystem(int sub, void* data, int dataSize);


	//---------------------登陆命令处理---------------------//
public:
	//发送登陆数据包
	virtual bool SendLogonPacket();
protected:
	//登陆成功
	virtual bool onSocketSubLogonSuccess(void* data, int dataSize);
	//登陆失败
	virtual bool onSocketSubLogonFailure(void* data, int dataSize);
	//登陆完成
	virtual bool onSocketSubLogonFinish(void* data, int dataSize);
	//更新
	virtual bool onSocketSubUpdateNotify(void* data, int dataSize);

	//---------------------配置命令处理---------------------//
protected:
	//列表配置
	virtual bool onSocketSubConfigColumu(void* data, int dataSize);
	//房间配置
	virtual bool onSocketSubConfigRoom(void* data, int dataSize);
	//道具配置
	virtual bool onSocketSubConfigProperty(void* data, int dataSize);
	//配置完成
	virtual bool onSocketSubConfigFinish(void* data, int dataSize);
	//玩家权限
	virtual bool onSocketSubConfigUserRight(void* data, int dataSize);

	//---------------------系统命令处理---------------------//
	
protected:
	bool onSocketSubSystemMessage(void* data, int dataSize);


	//---------------------用户命令处理---------------------//
public:
	//起立
	virtual bool SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave);
	//快速坐下(游戏服务器配置项 快速进入得打勾)
	virtual bool SendSitDownPacket();

protected:
	//用户进入
	virtual bool onSocketSubUserEnter(void* data, int dataSize);
	//用户状态
	virtual bool onSocketSubUserStatus(void* data, int dataSize);
	//错误信息
	virtual bool onSocketSubErrorCode(void* data, int dataSize);

	//---------------------状态命令处理---------------------//
public:
	//进入场景
	virtual bool SendGameOption();
	
protected:
	//桌子信息
	virtual bool onSocketSubStatusTableInfo(void* data, int dataSize);
	//桌子状态
	virtual bool onSocketSubStatusTableStatus(void* data, int dataSize);

	//---------------------游戏逻辑--------------------//
public:
	//发送心跳包
	virtual bool SendHeartPak();
	//请求房间数据
	virtual bool SendGameRoomInfo();
	//请求押注
	virtual bool SendJetInfo(word index, SCORE jet);
	//请求所有玩家总压分数
	virtual bool SendAllJetInfo();
protected:
	//接受心跳包
	virtual bool onSocketSubHeartPak(void* data, int dataSize);
	//得到金鲨银鲨游戏桌子信息
	virtual bool onSocketSubTableInfo(void* data, int dataSize);
	//返回所有玩家的压分信息
	virtual bool onSocketSubAllJetInfoRlt(void* data, int dataSize);
	//开奖消息处理
	virtual bool onSocketSubOpenRlt(void* data, int dataSize);
	//彩金开奖消息
	virtual bool onSocketSubOpenCJRlt(void* data, int dataSize);
	
	//开始游戏
	virtual bool onSocketSubStartGame(void* data, int dataSize);
	////有玩家加入或离开
	virtual bool onSocketSubJoinOrExit(void* data, int dataSize);
	//玩家压分信息同步通知
	virtual bool onSocketSubOtherUserJet(void* data, int dataSize);
	//通知刷新玩家列表
	virtual bool onSocketSubUpdateUserList(void* data, int dataSize);
	//有玩家取消了压分
	virtual bool onSocketSubOtherCancelJet(void* data, int dataSize);

public:
	static SharkNetWorkKernel* getInstance();
protected:
	static SharkNetWorkKernel* m_instance;
};



#endif // !_SharkNetWorkKernel_H
