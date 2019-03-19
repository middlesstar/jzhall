#ifndef SHZ_IClientKernel_H_
#define SHZ_IClientKernel_H_

#include "shuihu/plazz/kernel/IChatSink_SHZ.h"
#include "shuihu/plazz/kernel/IStringMessageSink_SHZ.h"
#include "IClientKernelSink_SHZ.h"

class HallGameServerItem;

//�ͻ����ں�
class SHZIClientKernel
{
public:
	virtual ~SHZIClientKernel(){};
	//////////////////////////////////////////////////////////////////////////
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetClientKernelSink(SHZIClientKernelSink* pIClientKernelSink)=0;
	//���ýӿ�
	virtual bool SetChatSink(SHZIChatSink* pIChatSink)=0;
	//���ýӿ�
	virtual bool SetStringMessageSink(SHZIStringMessageSink* pIStringMessageSink)=0;
	//��ʼ��
	virtual bool Init()=0;
	//�ر�
	virtual bool Intermit(int iExitCode)=0;

	//���Խӿ�
public:
	//�û�����
	virtual tagUserAttribute * GetUserAttribute()=0;
	//��Ϸ����
	virtual tagGameAttribute * GetGameAttribute()=0;
	//��������
	virtual tagServerAttribute * GetServerAttribute()=0;
	
	//ģʽ�ӿ�
public:
	//�Թ�״̬
	virtual bool IsLookonMode()=0;
	//����״̬
	virtual bool IsAllowLookon()=0;

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual byte GetGameStatus()=0;
	//����״̬
	virtual void SetGameStatus(byte cbGameStatus)=0;

	//�û��ӿ�
public:
	//�Լ�λ��
	virtual word GetMeChairID()=0;
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
	//�Թ��û�
	virtual SHZIClientUserItem * EnumLookonUserItem(word wEnumIndex)=0;

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID)=0;
	//���ͺ���
	virtual bool SendSocketData(word wMainCmdID, word wSubCmdID, void * pData, word wDataSize)=0;


	//���ܽӿ�
public:
	//���ͽ��볡��
	virtual bool SendGameOption()=0;
	//����׼��
	virtual bool SendUserReady(void * pData, word wDataSize)=0;
	//�Թۿ���
	virtual bool SendUserLookon(dword dwUserID, bool bAllowLookon)=0;
	//���ͱ���
	virtual bool SendUserExpression(dword dwTargetUserID, word wItemIndex)=0;
	//��������
	virtual bool SendUserChatMessage(dword dwTargetUserID, const char* pszChatString, dword crColor)=0;

	//�����ӿ�
public:
	//������
	virtual void ActiveGameFrame()=0;
	//�л�����
	virtual word SwitchViewChairID(word wChairID)=0;

	//////////////////////////////////////////////////////////////////////////
	//��ܽӿ�
	//////////////////////////////////////////////////////////////////////////
	//���ýӿ�
public:
	//��������
	virtual void OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagServerAttribute& ServerAttribute )=0;
	//�������
	virtual void OnGFConfigFinish()=0;

	//Socket�ӿ�
public:
	virtual bool OnGFEventSocket(int main, int sub, void* data, int dataSize)=0;

	//�û��ӿ�
public:
	//�û�����
	virtual void OnGFUserEnter(SHZIClientUserItem* pIClientUserItem)=0;
	//�û�����
	virtual void OnGFUserScore(dword dwUserID, const tagUserScore& UserScore)=0;
	//�û�״̬
	virtual void OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus)=0;
	//�û�����
	virtual void OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib)=0;
	//�û��Զ���ͷ��
	virtual void OnGFUserCustomFace(dword dwUserID,  dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo)=0;

public:
	static SHZIClientKernel* create();
	static void destory();
	static SHZIClientKernel* get();
};

#endif // _IClientKernel_H_