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

	void requestVisitorLogin();												//�����ο͵�¼
	void requestRegister(const string& strAcc, const string& strPsw);		//ע������
	void requestLogin(const string& strAcc, const string& strPsw);			//��¼����

	void requsetServerList();											//����������б�

	void requsetFlushServerInfo();										//����������ˢ��һ�·���������
	void onFlushServerInfoResp(const PacketAide& packet, int nLen);		

	void onLogonSuccess(const PacketAide& packet, int nLen);			//��¼�ɹ�
	void onLogonHallInfo(const PacketAide& packet, int nLen);			//��¼�󷵻صĴ�����Ϣ
	void onGameType(const PacketAide& packet, int nLen);
	void onGameKind(const PacketAide& packet, int nLen);
	// 	void onGamePage(const PacketAide& packet, int nLen);
	// 	void onGameNode(const PacketAide& packet, int nLen);
	void onGameServer(const PacketAide& packet, int nLen);
	void onLogonFinish(const PacketAide& packet, int nLen);			//��¼����

	void onLogonFailure(const PacketAide& packet, int nLen);		//��¼ʧ��

	void requestModifyIcon(int nFaceId);							//�����޸�ͷ��
	void onModifyIconResp(const PacketAide& packet, int nLen);		//�޸�ͷ�񷵻�

	void requestModifyNickname(const string& strName);				//�����޸��ǳ�
	void onModifyNicknameResp(const PacketAide& packet, int nLen);	//�޸��ǳƷ���

	void requestSaveCoinToBank(SCORE lScore);						//�����Ǯ������
	void requestTakeCoinFromBank(SCORE lScore);						//���������ȡ��
	void onBankOperateSuccess(const PacketAide& packet, int nLen);	//���в����ɹ�
	void onBankOperateFailure(const PacketAide& packet, int nLen);	//���в���ʧ��

	void requestSendTicket(dword gameid,dword tickets);				//�������ʹ�Ʊ
	void onSendTicketResp(const PacketAide& packet, int nLen);		//���ʹ�Ʊ����

	void requestModifyAccount(const string& strAccount,const string& strPassword);				//�����޸��˺�����
	void onModifyAccountResp(const PacketAide& packet, int nLen);								//�޸��˺����뷵��

	//todosl
	void onUpdateNotify(const PacketAide& packet, int nLen);								//ǿ�Ƹ���
public:
	void setCoin(int nCoinNum);
	void setGold(int nGoldNum);
	void setComment(bool bComment);

	void setVipLevel(int VipLevel);
	void setTotalRecharge(int nRechargeNum);
	void setShopItemRecharged(int nIndex);	//�����̵���Ʒ�ѳ�ֵ
	bool getShopItemRecharged(int nIndex);	//��ȡ�̵���Ʒ�Ƿ��ѳ�ֵ��
	int getNextVipNeedRecharge();			//���㵽����һ��VIP��Ҫ��ֵ����Ŀ

	void addCoin(int nCoinNum,bool bPlayEffect = true);
	void addGold(int nGoldNum);
	void addTicket(int nTicketNum);

	float getCurVipRechargePercent();		//��ȡ��ǰVIP�ȼ���ֵ�ٷֱ�
	int getPlayerLevel();					//��ȡ��ҵȼ�
	string getEncryptPassword();			//��ȡ���ܵ�����

	bool isVisitor();						//�Ƿ��ο�
public:
	void pushDataToSHZ();			//����������ͬ����ˮ䰴�
protected:
	void _setSubGameGlobalData_SHZ(const CMD_GP_LogonSuccess& logonData);
protected:
	CC_SYNTHESIZE(int, m_nVersion, Version);		//�汾��
	CC_SYNTHESIZE(int, m_nIosFlag, IosFlag);		//ios���ο���

	CC_SYNTHESIZE(CMD_GP_LogonSuccess, m_objLogonData, LogonData);
	CC_SYNTHESIZE(CMD_GP_LogonSuccess_Hall, m_objHallData, HallData);
	//CC_SYNTHESIZE(tagHallGameType, m_objGameType, GameType);
	//CC_SYNTHESIZE(tagHallGameKind, m_objGameKind, GameKind);
	//CC_SYNTHESIZE(tagHallGamePage, m_objGamePage, GamePage);
	//CC_SYNTHESIZE(tagHallGameNode, m_objGameNode, GameNode);
	//CC_SYNTHESIZE(tagHallGameServer, m_objGameServer, GameServer);
	CC_SYNTHESIZE(CMD_GP_LogonFinish, m_objLogonFinish, LogonFinish);
	CC_SYNTHESIZE(CMD_GP_LogonFailure, m_objLogonFailure, LogonFailure);

	CC_SYNTHESIZE(string, m_strAccount, Account);					//��ҵ�½�˺�
	CC_SYNTHESIZE(string, m_strPassword, Password);					//��ҵ�½����
	CC_SYNTHESIZE(string, m_strModifyName, ModifyName);				//��������޸��ǳƵĻ���
	CC_SYNTHESIZE(string, m_strModifyPassword, ModifyPassword);		//��������޸�����Ļ���
	CC_SYNTHESIZE(int, m_nSendTicketCounts, SendTicketCounts);		//����������ʹ�Ʊ������

	//todosl    
	CC_SYNTHESIZE(CMD_GP_UpdateNotify, m_objUpdateNotify, UpdateNotify);        //ǿ�Ƹ���


};

#endif // _Player_H_

