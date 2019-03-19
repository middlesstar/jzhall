#ifndef SHZ_IServerItem_H_
#define SHZ_IServerItem_H_

#include "shuihu/plazz/kernel/IChatSink_SHZ.h"
#include "shuihu/plazz/kernel/IStringMessageSink_SHZ.h"
#include "IServerItemSink_SHZ.h"


//查找桌子
struct tagSearchTable_SHZ
{
	bool							bOneNull;							//一个空位
	bool							bTwoNull;							//两个空位
	bool							bAllNull;							//全空位置
	bool							bNotFull;							//不全满位
	bool							bFilterPass;						//过滤密码
	word							wStartTableID;						//起始桌子
	word							wResultTableID;						//结果桌子
	word							wResultChairID;						//结果椅子
};

class HallGameServerItem;


//房间
class SHZIServerItem
{
public:
	virtual ~SHZIServerItem(){};

	//配置接口
public:
	//设置接口
	virtual bool SetServerItemSink(SHZIServerItemSink* pIServerItemSink)=0;
	//设置接口
	virtual bool SetChatSink(SHZIChatSink* pIChatSink)=0;
	//设置接口
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)=0;
	
	//连接接口
public:
	//连接服务器
	virtual bool ConnectServer(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)=0;
	//连接断开
	virtual bool IntermitConnect(bool force)=0;

	//属性接口
public:
	//用户属性
	virtual const tagUserAttribute& GetUserAttribute() const = 0;
	//房间属性
	virtual const tagServerAttribute&  GetServerAttribute() const = 0;
	//服务状态
	virtual SHZ_enServiceStatus GetServiceStatus()=0;
	
	//用户接口
public:
	//自己位置
	virtual SHZIClientUserItem * GetMeUserItem()=0;
	//游戏用户
	virtual SHZIClientUserItem * GetTableUserItem(word wChariID)=0;
	//查找用户
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID)=0;
	//查找用户
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID)=0;
	//查找用户
	virtual SHZIClientUserItem * SearchUserByNickName(const char* szNickName)=0;
	
	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID)=0;
	//发送函数
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * pData, word wDataSize)=0;

	//动作处理
public:
	//执行快速加入
	virtual bool PerformQuickSitDown()=0;
	//执行旁观
	virtual bool PerformLookonAction(word wTableID, word wChairID)=0;
	//执行起立
	virtual bool PerformStandUpAction(word wTableID, word wChairID)=0;
	//执行坐下
	virtual bool PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass)=0;
	//执行购买
	virtual bool PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)=0;

	//////////////////////////////////////////////////////////////////////////
	// 框架消息
public:
	//游戏已准备好
	virtual void OnGFGameReady()=0;
	//游戏关闭
	virtual void OnGFGameClose(int iExitCode)=0;

	//////////////////////////////////////////////////////////////////////////
	// Socket消息
public:
	// 发送数据
	virtual bool GFSendData(int main, int sub, void* data, int size)=0;

public:
	static SHZIServerItem* create();
	static void destory();
	static SHZIServerItem* get();
};

#endif // _IServerItem_H_