#ifndef SHZ_IServerItem_H_
#define SHZ_IServerItem_H_

#include "shuihu/plazz/kernel/IChatSink_SHZ.h"
#include "shuihu/plazz/kernel/IStringMessageSink_SHZ.h"
#include "IServerItemSink_SHZ.h"


//��������
struct tagSearchTable_SHZ
{
	bool							bOneNull;							//һ����λ
	bool							bTwoNull;							//������λ
	bool							bAllNull;							//ȫ��λ��
	bool							bNotFull;							//��ȫ��λ
	bool							bFilterPass;						//��������
	word							wStartTableID;						//��ʼ����
	word							wResultTableID;						//�������
	word							wResultChairID;						//�������
};

class HallGameServerItem;


//����
class SHZIServerItem
{
public:
	virtual ~SHZIServerItem(){};

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetServerItemSink(SHZIServerItemSink* pIServerItemSink)=0;
	//���ýӿ�
	virtual bool SetChatSink(SHZIChatSink* pIChatSink)=0;
	//���ýӿ�
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)=0;
	
	//���ӽӿ�
public:
	//���ӷ�����
	virtual bool ConnectServer(HallGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)=0;
	//���ӶϿ�
	virtual bool IntermitConnect(bool force)=0;

	//���Խӿ�
public:
	//�û�����
	virtual const tagUserAttribute& GetUserAttribute() const = 0;
	//��������
	virtual const tagServerAttribute&  GetServerAttribute() const = 0;
	//����״̬
	virtual SHZ_enServiceStatus GetServiceStatus()=0;
	
	//�û��ӿ�
public:
	//�Լ�λ��
	virtual SHZIClientUserItem * GetMeUserItem()=0;
	//��Ϸ�û�
	virtual SHZIClientUserItem * GetTableUserItem(word wChariID)=0;
	//�����û�
	virtual SHZIClientUserItem * SearchUserByUserID(dword dwUserID)=0;
	//�����û�
	virtual SHZIClientUserItem * SearchUserByGameID(dword dwGameID)=0;
	//�����û�
	virtual SHZIClientUserItem * SearchUserByNickName(const char* szNickName)=0;
	
	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID)=0;
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * pData, word wDataSize)=0;

	//��������
public:
	//ִ�п��ټ���
	virtual bool PerformQuickSitDown()=0;
	//ִ���Թ�
	virtual bool PerformLookonAction(word wTableID, word wChairID)=0;
	//ִ������
	virtual bool PerformStandUpAction(word wTableID, word wChairID)=0;
	//ִ������
	virtual bool PerformSitDownAction(word wTableID, word wChairID, bool bEfficacyPass)=0;
	//ִ�й���
	virtual bool PerformBuyProperty(byte cbRequestArea,const char* pszNickName, word wItemCount, word wPropertyIndex)=0;

	//////////////////////////////////////////////////////////////////////////
	// �����Ϣ
public:
	//��Ϸ��׼����
	virtual void OnGFGameReady()=0;
	//��Ϸ�ر�
	virtual void OnGFGameClose(int iExitCode)=0;

	//////////////////////////////////////////////////////////////////////////
	// Socket��Ϣ
public:
	// ��������
	virtual bool GFSendData(int main, int sub, void* data, int size)=0;

public:
	static SHZIServerItem* create();
	static void destory();
	static SHZIServerItem* get();
};

#endif // _IServerItem_H_