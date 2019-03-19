#ifndef SHZ_CServerItem_H_
#define SHZ_CServerItem_H_

#include "shuihu/plazz/Plazz_SHZ.h"
#include "Network/ISocketEngine.h"
#include "shuihu/plazz/kernel/user/ClientUserManager_SHZ.h"
#include "shuihu/plazz/kernel/IRoomFrameSink_SHZ.h"
#include "shuihu/plazz/kernel/IGameFrameSink_SHZ.h"
#include "IServerItem_SHZ.h"
#include "TableViewFrame_SHZ.h"
#include "Network/CommandStructDef_SHZ.h"
#include "shuihu/View/Scene/LotteryScene/LotteryMission_SHZ.h"

class CUserManager;

class SHZCServerItem 
	: public SHZIServerItem
	, public SHZIUserManagerSink
	, public ISocketEngineSink
	//, public IRoomFrameSink
{

public:
	SHZCServerItem();
	~SHZCServerItem();
	//////////////////////////////////////////////////////////////////////////
	// IServerItem
	//////////////////////////////////////////////////////////////////////////
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetServerItemSink(SHZIServerItemSink* pIServerItemSink);
	//���ýӿ�
	virtual bool SetChatSink(SHZIChatSink* pIChatSink);
	//���ýӿ�
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink);
	//���÷���
	virtual bool SetServerAttribute(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr);

	//���ӽӿ�
public:
	//��ʼ������
	virtual bool ConnectServer(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr);
	//�ж�����
	virtual bool IntermitConnect(bool force);
	

	//���Խӿ�
public:
	//�û�����
	virtual const tagUserAttribute& GetUserAttribute() const;
	//��������
	virtual const tagServerAttribute&  GetServerAttribute() const;
	//����״̬
	virtual SHZ_enServiceStatus GetServiceStatus();
	//�Ƿ����״̬
	bool IsService();
	//����״̬
	void SetServiceStatus(SHZ_enServiceStatus ServiceStatus);
	//�Լ�״̬
	bool IsPlayingMySelf();
	
	//�û��ӿ�
public:
	//�Լ�λ��
	virtual word GetMeChairID();
	//�Լ�λ��
	virtual SHZIClientUserItem * GetMeUserItem();
	//��Ϸ�û�
	virtual SHZIClientUserItem * GetTableUserItem(word wChariID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID);
	//�����û�
	virtual SHZIClientUserItem * SearchUserByNickName(const char* szNickName);
	
	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID);
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize);

	//��������
public:
	//���͵�¼
	virtual bool SendLogonPacket();
	//���͹���
	virtual bool SendUserRulePacket();
	//�����Թ�
	virtual bool SendLookonPacket(word wTableID, word wChairID);
	//��������
	virtual bool SendSitDownPacket(word wTableID, word wChairID, const char* lpszPassword=0);
	//��������
	virtual bool SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave);
	//���ͱ���
	virtual bool SendExpressionPacket(dword dwTargetUserID, word wItemIndex);
	//��������
	virtual bool SendUserChatPacket(dword dwTargetUserID, const char* pszChatString, dword dwColor);

	//��������
public:
	//��������
	virtual bool SearchGameTable(tagSearchTable_SHZ & SearchTable);
	//ִ�п��ټ���
	virtual bool PerformQuickSitDown();
	//ִ���Թ�
	virtual bool PerformLookonAction(word wTableID, word wChairID);
	//ִ������
	virtual bool PerformStandUpAction(word wTableID, word wChairID);
	//ִ������
	virtual bool PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass);
	//ִ�й���
	virtual bool PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex);

	//�ڲ�����
protected:
	//����Ч��
	bool EfficacyUserChat(const char* pszChatString, word wExpressionIndex);
	//����Ч��
	bool EfficacyTableRule(word wTableID, word wChairID, bool bReqLookon, char strDescribe[256]);

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
	// ISocketEngineSink
	//////////////////////////////////////////////////////////////////////////
	//

public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//��¼��Ϣ
	bool OnSocketMainLogon(int sub, void* data, int dataSize);
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(void* data, int dataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(void* data, int dataSize);
	//��¼���
	bool OnSocketSubLogonFinish(void* data, int dataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(void* data, int dataSize);
	
	//////////////////////////////////////////////////////////////////////////
	//������Ϣ
	bool OnSocketMainConfig(int sub, void* data, int dataSize);
	//�б�����
	bool OnSocketSubConfigColumn(void* data, int dataSize);
	//��������
	bool OnSocketSubConfigServer(void* data, int dataSize);
	//��������
	bool OnSocketSubConfigProperty(void* data, int dataSize);
	//�������Ȩ��	
	bool OnSocketSubConfigUserRight(void* data, int dataSize);
	//�������
	bool OnSocketSubConfigFinish(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//�û���Ϣ
	bool OnSocketMainUser(int sub, void* data, int dataSize);
	//����ʧ��
	bool OnSocketSubRequestFailure(void* data, int dataSize);
	//�������
	bool OnSocketSubUserInvite(void* data, int dataSize);
	//�ȴ�����
	bool OnSocketSubWaitDistribute(void* data, int dataSize);
	//�û�����
	bool OnSocketSubUserEnter(void* data, int dataSize);
	//�û�����
	bool OnSocketSubUserScore(void* data, int dataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(void* data, int dataSize);
	//�û�����
	bool OnSocketSubUserChat(void* data, int dataSize);
	//�û�����
	bool OnSocketSubExpression(void* data, int dataSize);
	//�û�˽��
	bool OnSocketSubWisperUserChat(void* data, int dataSize);
	//˽�ı���
	bool OnSocketSubWisperExpression(void* data, int dataSize);
	//���߳ɹ�
	bool OnSocketSubPropertySuccess(void* data, int dataSize);
	//����ʧ��
	bool OnSocketSubPropertyFailure(void* data, int dataSize);
	//����ЧӦ
	bool OnSocketSubPropertyEffect(void* data, int dataSize);
	//������Ϣ
	bool OnSocketSubPropertyMessage(void* data, int dataSize);
	//��������
	bool OnSocketSubPropertyTrumpet(void* data, int dataSize);
	//ϲ����Ϣ
	bool OnSocketSubGladMessage(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//״̬��Ϣ
	bool OnSocketMainStatus(int sub, void* data, int dataSize);
	//������Ϣ
	bool OnSocketSubStatusTableInfo(void* data, int dataSize);
	//����״̬
	bool OnSocketSubStatusTableStatus(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//������Ϣ
	bool OnSocketMainInsure(int sub, void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//������Ϣ
	bool OnSocketMainManager(int sub, void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//ϵͳ��Ϣ
	bool OnSocketMainSystem(int sub, void* data, int dataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(void* data, int dataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(void* data, int dataSize);

	//////////////////////////////////////////////////////////////////////////
	//��Ϸ��Ϣ,�����Ϣ
	bool OnSocketMainGameFrame(int main,int sub, void* data, int dataSize);
	
	//////////////////////////////////////////////////////////////////////////
	//������Ϣ
	bool OnSocketMainMatch(int sub, void* data, int dataSize);
	//��������
	bool OnSocketSubMatchFee(void* data, int dataSize);
	//��������
	bool OnSocketSubMatchNum(void* data, int dataSize);
	//����״̬
	bool OnSocketSubMatchStatus(void* data, int dataSize);
	//��������
	bool OnSocketSubMatchDesc(void* data, int dataSize);


	//////////////////////////////////////////////////////////////////////////
	// �����Ϣ
public:
	//��Ϸ��׼����
	virtual void OnGFGameReady();
	//��Ϸ�ر�
	virtual void OnGFGameClose(int iExitCode);

	//////////////////////////////////////////////////////////////////////////
	// Socket��Ϣ
public:
	// ��������
	virtual bool GFSendData(int main, int sub, void* data, int size);

	//////////////////////////////////////////////////////////////////////////
	// ����
	//////////////////////////////////////////////////////////////////////////
private:
	//��������
	word	mReqTableID;						//��������
	word	mReqChairID;						//����λ��
	word	mFindTableID;						//��������
	bool	mIsGameReady;						//��Ϸ�Ƿ�׼����
	bool	mIsQuickSitDown;					//�Ƿ���ټ������

	//�û�
	SHZIClientUserItem*	mMeUserItem;
	SHZCPlazaUserManager*	mUserManager;

	//��������
	SHZ_enServiceStatus		mServiceStatus;
	tagGameKind			mGameKind;						//������Ϣ
	tagSHZ_GameServer		mGameServer;					//������Ϣ
	tagUserAttribute	mUserAttribute;					//�û�����
	tagServerAttribute	mServerAttribute;				//��������

	//���ò���
	SHZCParameterGame*		mParameterGame;					//��Ϸ����
	SHZCParameterServer*	mParameterServer;				//��������
	
	//����
	SHZCTableViewFrame		mTableViewFrame;				//���ӿ��

	//�ӿ�
	SHZIServerItemSink*	mIServerItemSink;				//����ӿ�
	SHZIChatSink*			mIChatSink;						//����ӿ�
	SHZIStringMessageSink*	mIStringMessageSink;			//��Ϣ�ӿ�
	
	// ��������
	ISocketEngine*		mSocketEngine;
};
#endif // _CServerItem_H_