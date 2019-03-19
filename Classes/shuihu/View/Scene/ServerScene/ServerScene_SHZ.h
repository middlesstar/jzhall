#ifndef SHZ_ServerScene_H_
#define SHZ_ServerScene_H_

#include "cocos2d.h"

class SHZChatLayer;


//游戏房间
class SHZServerScene
	: public cocos2d::CCScene
	, public SHZIServerItemSink
{
public:
	static SHZServerScene* create(HallGameServerItem* pGameServerItem);
private:
	SHZServerScene();
	~SHZServerScene();
	bool init(HallGameServerItem* pGameServerItem);
public:
	virtual void onEnterTransitionDidFinish();

private:
	void onQuickSitdown(float dt);
	
	//////////////////////////////////////////////////////////////////////////
	// IServerItemSink
	////////////////////////////////////////////////////////////////////////// 
	//登陆信息
public:
	//坐下失败
	virtual void onGRRequestFailure(const std::string& sDescribeString);
	//登陆成功
	virtual void OnGRLogonSuccess();
	//登陆失败
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString);
	//登陆完成
	virtual void OnGRLogonFinish();
	//更新通知
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString);
	void OnUpdateRequest(Node* node);

	//配置信息
public:
	//列表配置
	virtual void OnGRConfigColumn();
	//房间配置
	virtual void OnGRConfigServer();
	//道具配置
	virtual void OnGRConfigProperty();
	//玩家权限配置
	virtual void OnGRConfigUserRight() ;
	//配置完成
	virtual void OnGRConfigFinish();

	//用户信息
public:
	//用户进入
	virtual void OnGRUserEnter(SHZIClientUserItem* pIClientUserItem);
	//用户更新
	virtual void OnGRUserUpdate(SHZIClientUserItem* pIClientUserItem);
	//用户删除
	virtual void OnGRUserDelete(SHZIClientUserItem* pIClientUserItem);

	//框架消息
public:
	//用户邀请
	virtual void OnGFUserInvite(const std::string& szMessage);
	//用户邀请失败
	virtual void OnGFUserInviteFailure(const std::string& szMessage);
	//房间退出
	virtual void OnGFServerClose(const std::string& szMessage);

	//创建游戏内核
	virtual bool CreateKernel();

	//辅助函数
public:
	void func_server_close(float dt);
private:
	HallGameServerItem*	mGameServerItem;
	SHZIServerItem*		mServerItem;
	SHZChatLayer*			mChatLayer;

	std::string			mMessageTemp;
};
#endif // _ServerScene_H_