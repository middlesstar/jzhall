#ifndef SHZ__CClientKernel_H_
#define SHZ__CClientKernel_H_

#include "shuihu/plazz/Plazz_SHZ.h"
#include "Network/ISocketEngine.h"
#include "shuihu/plazz/kernel/user/ClientUserManager_SHZ.h"
#include "shuihu/plazz/kernel/IRoomFrameSink_SHZ.h"
#include "shuihu/plazz/kernel/IGameFrameSink_SHZ.h"
#include "IClientKernel_SHZ.h"

class SHZCClientKernel 
	: public SHZIClientKernel
	, public SHZIUserManagerSink
	//, public IGameFrameSink
{

public:
	SHZCClientKernel();
	~SHZCClientKernel();

	//////////////////////////////////////////////////////////////////////////
	// IClientKernel
	//配置接口
public:
	//设置接口
	virtual bool SetClientKernelSink(SHZIClientKernelSink* pIClientKernelSink);
	//设置接口
	virtual bool SetChatSink(SHZIChatSink* pIChatSink);
	//设置接口
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink);
	//内核配置
	virtual bool Init();
	//关闭
	virtual bool Intermit(int iExitCode);

	//属性接口
public:
	//用户属性
	virtual tagUserAttribute * GetUserAttribute();
	//游戏属性
	virtual tagGameAttribute * GetGameAttribute();
	//房间属性
	virtual tagServerAttribute * GetServerAttribute();
		
	//模式接口
public:
	//旁观状态
	virtual bool IsLookonMode();
	//允许状态
	virtual bool IsAllowLookon();

	//状态接口
public:
	//获取状态
	virtual byte GetGameStatus();
	//设置状态
	virtual void SetGameStatus(byte cbGameStatus);

	//用户接口
public:
	//自己位置
	virtual word GetMeChairID();
	//自己位置
	virtual SHZIClientUserItem * GetMeUserItem();
	//游戏用户
	virtual SHZIClientUserItem * GetTableUserItem(word wChariID);
	//查找用户
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID);
	//查找用户
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID);
	//查找用户
	virtual SHZIClientUserItem * SearchUserByNickName(const char* szNickName);
	//旁观用户
	virtual SHZIClientUserItem * EnumLookonUserItem(word wEnumIndex);

	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID);
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize);



	//功能接口
public:
	//发送进入场景
	virtual bool SendGameOption();
	//发送准备
	virtual bool SendUserReady(void * data, word dataSize);
	//旁观控制
	virtual bool SendUserLookon(dword dwUserID, bool bAllowLookon);
	//发送表情
	virtual bool SendUserExpression(dword dwTargetUserID, word wItemIndex);
	//发送聊天
	virtual bool SendUserChatMessage(dword dwTargetUserID, const char* pszChatString, dword crColor);

	//辅助接口
public:
	//激活框架
	virtual void ActiveGameFrame();
	//切换椅子
	virtual word SwitchViewChairID(word wChairID);


	//////////////////////////////////////////////////////////////////////////
	// IUserManagerSink
	//////////////////////////////////////////////////////////////////////////
public:
	virtual void OnUserItemAcitve(SHZIClientUserItem* pIClientUserItem);
	virtual void OnUserItemDelete(SHZIClientUserItem* pIClientUserItem);
	virtual void OnUserFaceUpdate(SHZIClientUserItem* pIClientUserItem);
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserScore& LastScore);
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus);
	virtual void OnUserItemUpdate(SHZIClientUserItem* pIClientUserItem, const tagUserAttrib & UserAttrib);

	//////////////////////////////////////////////////////////////////////////
	//IGameFrameSink
	
	//配置接口
public:
	//房间配置
	virtual void OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagServerAttribute& ServerAttribute);
	//配置完成
	virtual void OnGFConfigFinish();

	//Socket接口
public:
	virtual bool OnGFEventSocket(int main, int sub, void* data, int dataSize);

	//用户聊天
	bool OnSocketSubUserChat(void* data, int dataSize);
	//用户表情
	bool OnSocketSubExpression(void* data, int dataSize);
	//游戏状态
	bool OnSocketSubGameStatus(void* data, int dataSize);
	//游戏场景
	bool OnSocketSubGameScene(void* data, int dataSize);
	//旁观状态
	bool OnSocketSubLookonStatus(void* data, int dataSize);
	//系统消息
	bool OnSocketSubSystemMessage(void* data, int dataSize);
	//动作消息
	bool OnSocketSubActionMessage(void* data, int dataSize);

	//用户接口
public:
	//用户进入
	virtual void OnGFUserEnter(SHZIClientUserItem* pIClientUserItem);
	//用户积分
	virtual void OnGFUserScore(dword dwUserID, const tagUserScore& UserScore);
	//用户状态
	virtual void OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus);
	//用户属性
	virtual void OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib);
	//用户自定义头像
	virtual void OnGFUserCustomFace(dword dwUserID,  dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo);

	//////////////////////////////////////////////////////////////////////////
	// 数据
	//////////////////////////////////////////////////////////////////////////
private:
	bool mAllowLookon;									//是否允许旁观
	byte mGameStatus;									//游戏状态

	SHZIClientUserItem*	mMeUserItem;					//自身数据
	SHZCGameUserManager*	mUserManager;					//用户管理

	tagUserAttribute	mUserAttribute;					//用户属性
	tagGameAttribute	mGameAttribute;					//游戏属性
	tagServerAttribute	mServerAttribute;				//房间属性
	
	//接口
	SHZIClientKernelSink*	mIClientKernelSink;				//内核接口
	SHZIChatSink*			mIChatSink;						//聊天接口
	SHZIStringMessageSink*	mIStringMessageSink;			//信息接口

};
#endif // _CClientKernel_H_