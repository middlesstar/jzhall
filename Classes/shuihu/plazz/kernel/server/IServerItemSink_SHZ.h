#ifndef SHZ_IServerItemSink_H_
#define SHZ_IServerItemSink_H_

#include "shuihu/plazz/kernel/user/IClientUserItem_SHZ.h"

//服务状态
enum SHZ_enServiceStatus
{
	SHZ_ServiceStatus_Unknow,			//未知状态
	SHZ_ServiceStatus_Entering,			//进入状态
	SHZ_ServiceStatus_Validate,			//验证状态
	SHZ_ServiceStatus_RecvInfo,			//读取状态
	SHZ_ServiceStatus_ServiceIng,		//服务状态
	SHZ_ServiceStatus_NetworkDown,		//中断状态
};

//游戏退出代码
enum SHZ_enGameExitCode
{
	SHZ_GameExitCode_Normal,			//正常退出
	SHZ_GameExitCode_CreateFailed,		//创建失败
	SHZ_GameExitCode_Timeout,			//定时到时
	SHZ_GameExitCode_Shutdown,			//断开连接
};

//房间退出代码
enum SHZ_enServerExitCode
{
	SHZ_ServerExitCode_Normal,			//正常退出
	SHZ_ServerExitCode_Shutdown,		//断开连接
};

class SHZIServerItemSink
{
public:
	virtual ~SHZIServerItemSink(){};

	//登陆信息
public:
	//请求失败
	virtual void onGRRequestFailure(const std::string& sDescribeString) = 0;
	//登陆成功
	virtual void OnGRLogonSuccess() = 0;
	//登陆失败
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString) = 0;
	//登陆完成
	virtual void OnGRLogonFinish() = 0;
	//更新通知
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString)=0;

	//配置信息
public:
	//列表配置
	virtual void OnGRConfigColumn() = 0;
	//房间配置
	virtual void OnGRConfigServer() = 0;
	//道具配置
	virtual void OnGRConfigProperty() = 0;
	//玩家权限配置
	virtual void OnGRConfigUserRight() = 0;
	//配置完成
	virtual void OnGRConfigFinish() = 0;
	
	//用户信息
public:
	//用户进入
	virtual void OnGRUserEnter(SHZIClientUserItem* pIClientUserItem)=0;
	//用户更新
	virtual void OnGRUserUpdate(SHZIClientUserItem* pIClientUserItem) = 0;
	//用户删除
	virtual void OnGRUserDelete(SHZIClientUserItem* pIClientUserItem)=0;

	//框架消息
public:
	//用户邀请
	virtual void OnGFUserInvite(const std::string& szMessage)=0;
	//用户邀请失败
	virtual void OnGFUserInviteFailure(const std::string& szMessage)=0;
	//房间退出
	virtual void OnGFServerClose(const std::string& szMessage)=0;
	//创建游戏内核
	virtual bool CreateKernel() = 0;
};

#endif // _IServerItemSink_H_