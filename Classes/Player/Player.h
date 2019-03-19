#ifndef  _Player_H_
#define  _Player_H_

#include "MacroDefineCommon.h"
#include "Network/CommandDef.h"
#include "Network/CommandStructDef.h"
#include "Network/NetManager.h"

#include "network/HttpClient.h"

class  Player
{
public:
	Player();
	virtual ~Player();

	SINGLE_INSTANCE_FUNC(Player);

protected:
	void _registerMessageHandler();

public:
	void requestVersionInfo();
	void onVersionInfoResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void requestVisitorLogin();												//请求游客登录
	void requestRegister(const string& strAcc, const string& strPsw);		//注册请求
	void requestLogin(const string& strAcc, const string& strPsw);			//登录请求

	void requsetServerList();											//请求服务器列表

	void requsetFlushServerInfo();										//断线重连后刷新一下服务器数据
	void onFlushServerInfoResp(const PacketAide& packet, int nLen);		

	void onLogonSuccess(const PacketAide& packet, int nLen);			//登录成功
	void onLogonHallInfo(const PacketAide& packet, int nLen);			//登录后返回的大厅信息
	void onGameType(const PacketAide& packet, int nLen);
	void onGameKind(const PacketAide& packet, int nLen);
	// 	void onGamePage(const PacketAide& packet, int nLen);
	// 	void onGameNode(const PacketAide& packet, int nLen);
	void onGameServer(const PacketAide& packet, int nLen);
	void onLogonFinish(const PacketAide& packet, int nLen);			//登录结束

	void onLogonFailure(const PacketAide& packet, int nLen);		//登录失败

	void requestModifyIcon(int nFaceId);							//请求修改头像
	void onModifyIconResp(const PacketAide& packet, int nLen);		//修改头像返回

	void requestModifyNickname(const string& strName);				//请求修改昵称
	void onModifyNicknameResp(const PacketAide& packet, int nLen);	//修改昵称返回

	void requestSaveCoinToBank(SCORE lScore);						//请求存钱到银行
	void requestTakeCoinFromBank(SCORE lScore);						//请求从银行取款
	void onBankOperateSuccess(const PacketAide& packet, int nLen);	//银行操作成功
	void onBankOperateFailure(const PacketAide& packet, int nLen);	//银行操作失败

	void requestSendTicket(dword gameid,dword tickets);				//请求赠送船票
	void onSendTicketResp(const PacketAide& packet, int nLen);		//赠送船票返回

	void requestModifyAccount(const string& strAccount,const string& strPassword);				//请求修改账号密码
	void onModifyAccountResp(const PacketAide& packet, int nLen);								//修改账号密码返回

	//todosl
	void onUpdateNotify(const PacketAide& packet, int nLen);								//强制更新
public:
	void setCoin(int nCoinNum);
	void setGold(int nGoldNum);
	void setComment(bool bComment);

	void setVipLevel(int VipLevel);
	void setTotalRecharge(int nRechargeNum);
	void setShopItemRecharged(int nIndex);	//设置商店物品已充值
	bool getShopItemRecharged(int nIndex);	//获取商店物品是否已充值过
	int getNextVipNeedRecharge();			//计算到达下一级VIP需要充值的数目

	void addCoin(int nCoinNum,bool bPlayEffect = true);
	void addGold(int nGoldNum);
	void addTicket(int nTicketNum);

	float getCurVipRechargePercent();		//获取当前VIP等级充值百分比
	int getPlayerLevel();					//获取玩家等级
	string getEncryptPassword();			//获取加密的密码

	bool isVisitor();						//是否游客
public:
	void pushDataToSHZ();			//将大厅数据同步到水浒传
protected:
	void _setSubGameGlobalData_SHZ(const CMD_GP_LogonSuccess& logonData);
protected:
	CC_SYNTHESIZE(int, m_nVersion, Version);		//版本号
	CC_SYNTHESIZE(int, m_nIosFlag, IosFlag);		//ios屏蔽开关

	CC_SYNTHESIZE(CMD_GP_LogonSuccess, m_objLogonData, LogonData);
	CC_SYNTHESIZE(CMD_GP_LogonSuccess_Hall, m_objHallData, HallData);
	//CC_SYNTHESIZE(tagHallGameType, m_objGameType, GameType);
	//CC_SYNTHESIZE(tagHallGameKind, m_objGameKind, GameKind);
	//CC_SYNTHESIZE(tagHallGamePage, m_objGamePage, GamePage);
	//CC_SYNTHESIZE(tagHallGameNode, m_objGameNode, GameNode);
	//CC_SYNTHESIZE(tagHallGameServer, m_objGameServer, GameServer);
	CC_SYNTHESIZE(CMD_GP_LogonFinish, m_objLogonFinish, LogonFinish);
	CC_SYNTHESIZE(CMD_GP_LogonFailure, m_objLogonFailure, LogonFailure);

	CC_SYNTHESIZE(string, m_strAccount, Account);					//玩家登陆账号
	CC_SYNTHESIZE(string, m_strPassword, Password);					//玩家登陆密码
	CC_SYNTHESIZE(string, m_strModifyName, ModifyName);				//玩家请求修改昵称的缓存
	CC_SYNTHESIZE(string, m_strModifyPassword, ModifyPassword);		//玩家请求修改密码的缓存
	CC_SYNTHESIZE(int, m_nSendTicketCounts, SendTicketCounts);		//玩家请求赠送船票的数量

	//todosl    
	CC_SYNTHESIZE(CMD_GP_UpdateNotify, m_objUpdateNotify, UpdateNotify);        //强制更新


};

#endif // _Player_H_

