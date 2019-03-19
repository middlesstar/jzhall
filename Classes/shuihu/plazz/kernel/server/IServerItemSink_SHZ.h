#ifndef SHZ_IServerItemSink_H_
#define SHZ_IServerItemSink_H_

#include "shuihu/plazz/kernel/user/IClientUserItem_SHZ.h"

//����״̬
enum SHZ_enServiceStatus
{
	SHZ_ServiceStatus_Unknow,			//δ֪״̬
	SHZ_ServiceStatus_Entering,			//����״̬
	SHZ_ServiceStatus_Validate,			//��֤״̬
	SHZ_ServiceStatus_RecvInfo,			//��ȡ״̬
	SHZ_ServiceStatus_ServiceIng,		//����״̬
	SHZ_ServiceStatus_NetworkDown,		//�ж�״̬
};

//��Ϸ�˳�����
enum SHZ_enGameExitCode
{
	SHZ_GameExitCode_Normal,			//�����˳�
	SHZ_GameExitCode_CreateFailed,		//����ʧ��
	SHZ_GameExitCode_Timeout,			//��ʱ��ʱ
	SHZ_GameExitCode_Shutdown,			//�Ͽ�����
};

//�����˳�����
enum SHZ_enServerExitCode
{
	SHZ_ServerExitCode_Normal,			//�����˳�
	SHZ_ServerExitCode_Shutdown,		//�Ͽ�����
};

class SHZIServerItemSink
{
public:
	virtual ~SHZIServerItemSink(){};

	//��½��Ϣ
public:
	//����ʧ��
	virtual void onGRRequestFailure(const std::string& sDescribeString) = 0;
	//��½�ɹ�
	virtual void OnGRLogonSuccess() = 0;
	//��½ʧ��
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString) = 0;
	//��½���
	virtual void OnGRLogonFinish() = 0;
	//����֪ͨ
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString)=0;

	//������Ϣ
public:
	//�б�����
	virtual void OnGRConfigColumn() = 0;
	//��������
	virtual void OnGRConfigServer() = 0;
	//��������
	virtual void OnGRConfigProperty() = 0;
	//���Ȩ������
	virtual void OnGRConfigUserRight() = 0;
	//�������
	virtual void OnGRConfigFinish() = 0;
	
	//�û���Ϣ
public:
	//�û�����
	virtual void OnGRUserEnter(SHZIClientUserItem* pIClientUserItem)=0;
	//�û�����
	virtual void OnGRUserUpdate(SHZIClientUserItem* pIClientUserItem) = 0;
	//�û�ɾ��
	virtual void OnGRUserDelete(SHZIClientUserItem* pIClientUserItem)=0;

	//�����Ϣ
public:
	//�û�����
	virtual void OnGFUserInvite(const std::string& szMessage)=0;
	//�û�����ʧ��
	virtual void OnGFUserInviteFailure(const std::string& szMessage)=0;
	//�����˳�
	virtual void OnGFServerClose(const std::string& szMessage)=0;
	//������Ϸ�ں�
	virtual bool CreateKernel() = 0;
};

#endif // _IServerItemSink_H_