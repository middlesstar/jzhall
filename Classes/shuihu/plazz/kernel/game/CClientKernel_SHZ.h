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
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetClientKernelSink(SHZIClientKernelSink* pIClientKernelSink);
	//���ýӿ�
	virtual bool SetChatSink(SHZIChatSink* pIChatSink);
	//���ýӿ�
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink);
	//�ں�����
	virtual bool Init();
	//�ر�
	virtual bool Intermit(int iExitCode);

	//���Խӿ�
public:
	//�û�����
	virtual tagUserAttribute * GetUserAttribute();
	//��Ϸ����
	virtual tagGameAttribute * GetGameAttribute();
	//��������
	virtual tagServerAttribute * GetServerAttribute();
		
	//ģʽ�ӿ�
public:
	//�Թ�״̬
	virtual bool IsLookonMode();
	//����״̬
	virtual bool IsAllowLookon();

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual byte GetGameStatus();
	//����״̬
	virtual void SetGameStatus(byte cbGameStatus);

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
	//�Թ��û�
	virtual SHZIClientUserItem * EnumLookonUserItem(word wEnumIndex);

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID);
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize);



	//���ܽӿ�
public:
	//���ͽ��볡��
	virtual bool SendGameOption();
	//����׼��
	virtual bool SendUserReady(void * data, word dataSize);
	//�Թۿ���
	virtual bool SendUserLookon(dword dwUserID, bool bAllowLookon);
	//���ͱ���
	virtual bool SendUserExpression(dword dwTargetUserID, word wItemIndex);
	//��������
	virtual bool SendUserChatMessage(dword dwTargetUserID, const char* pszChatString, dword crColor);

	//�����ӿ�
public:
	//������
	virtual void ActiveGameFrame();
	//�л�����
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
	
	//���ýӿ�
public:
	//��������
	virtual void OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagServerAttribute& ServerAttribute);
	//�������
	virtual void OnGFConfigFinish();

	//Socket�ӿ�
public:
	virtual bool OnGFEventSocket(int main, int sub, void* data, int dataSize);

	//�û�����
	bool OnSocketSubUserChat(void* data, int dataSize);
	//�û�����
	bool OnSocketSubExpression(void* data, int dataSize);
	//��Ϸ״̬
	bool OnSocketSubGameStatus(void* data, int dataSize);
	//��Ϸ����
	bool OnSocketSubGameScene(void* data, int dataSize);
	//�Թ�״̬
	bool OnSocketSubLookonStatus(void* data, int dataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(void* data, int dataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(void* data, int dataSize);

	//�û��ӿ�
public:
	//�û�����
	virtual void OnGFUserEnter(SHZIClientUserItem* pIClientUserItem);
	//�û�����
	virtual void OnGFUserScore(dword dwUserID, const tagUserScore& UserScore);
	//�û�״̬
	virtual void OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus);
	//�û�����
	virtual void OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib);
	//�û��Զ���ͷ��
	virtual void OnGFUserCustomFace(dword dwUserID,  dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo);

	//////////////////////////////////////////////////////////////////////////
	// ����
	//////////////////////////////////////////////////////////////////////////
private:
	bool mAllowLookon;									//�Ƿ������Թ�
	byte mGameStatus;									//��Ϸ״̬

	SHZIClientUserItem*	mMeUserItem;					//��������
	SHZCGameUserManager*	mUserManager;					//�û�����

	tagUserAttribute	mUserAttribute;					//�û�����
	tagGameAttribute	mGameAttribute;					//��Ϸ����
	tagServerAttribute	mServerAttribute;				//��������
	
	//�ӿ�
	SHZIClientKernelSink*	mIClientKernelSink;				//�ں˽ӿ�
	SHZIChatSink*			mIChatSink;						//����ӿ�
	SHZIStringMessageSink*	mIStringMessageSink;			//��Ϣ�ӿ�

};
#endif // _CClientKernel_H_