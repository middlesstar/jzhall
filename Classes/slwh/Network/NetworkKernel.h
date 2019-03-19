#ifndef SLWH_NETWORKKERNEL
#define SLWH_NETWORKKERNEL

#include "Network/CSocketMission.h"
#include "slwh/ResourcePath.h"

class NetworkKernel : 
	public CSocketMission
{
protected:

	NetworkKernel();

	NetworkKernel(const std::string& url, int port);

	virtual ~NetworkKernel();

public:
	//�ͷ�
	virtual void relese();

	//---------------------�������Ӵ�����---------------------//
public:
	//�������˷�����
	virtual void onEventTCPSocketLink();
	//�ر��˷�����
	virtual void onEventTCPSocketShut();
	//�������
	virtual void onEventTCPSocketError(int errorCode);
	//��·��ȡ
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

	//---------------------����Ϣ����---------------------//
protected:
	//��½����
	virtual bool onSubCmdLogon(int sub, void* data, int dataSize);
	//��������
	virtual bool onSubCmdConfig(int sub, void* data, int dataSize);
	//�û���Ϣ
	virtual bool onSunCmdUserInfo(int sub, void* data, int dataSize);
	//��Ϸ����
	virtual bool onSubCmdGame(int sub, void* data, int dataSize);
	//״̬����
	virtual bool onSubCmdState(int sub, void* data, int dataSize);
	//ϵͳ���� 
	virtual bool onSubCmdSystem(int sub, void* data, int dataSize);


	//---------------------��½�����---------------------//
public:
	//���͵�½���ݰ�
	virtual bool SendLogonPacket();
protected:
	//��½�ɹ�
	virtual bool onSocketSubLogonSuccess(void* data, int dataSize);
	//��½ʧ��
	virtual bool onSocketSubLogonFailure(void* data, int dataSize);
	//��½���
	virtual bool onSocketSubLogonFinish(void* data, int dataSize);
	//����
	virtual bool onSocketSubUpdateNotify(void* data, int dataSize);

	//---------------------���������---------------------//
protected:
	//�б�����
	virtual bool onSocketSubConfigColumu(void* data, int dataSize);
	//��������
	virtual bool onSocketSubConfigRoom(void* data, int dataSize);
	//��������
	virtual bool onSocketSubConfigProperty(void* data, int dataSize);
	//�������
	virtual bool onSocketSubConfigFinish(void* data, int dataSize);
	//���Ȩ��
	virtual bool onSocketSubConfigUserRight(void* data, int dataSize);

	//---------------------ϵͳ�����---------------------//
	
protected:
	bool onSocketSubSystemMessage(void* data, int dataSize);


	//---------------------�û������---------------------//
public:
	//����
	virtual bool SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave);
	//��������(��Ϸ������������ ���ٽ���ô�)
	virtual bool SendSitDownPacket();

protected:
	//�û�����
	virtual bool onSocketSubUserEnter(void* data, int dataSize);
	//�û�״̬
	virtual bool onSocketSubUserStatus(void* data, int dataSize);
	//������Ϣ
	virtual bool onSocketSubErrorCode(void* data, int dataSize);

	//---------------------״̬�����---------------------//
public:
	//���볡��
	virtual bool SendGameOption();
	
protected:
	//������Ϣ
	virtual bool onSocketSubStatusTableInfo(void* data, int dataSize);
	//����״̬
	virtual bool onSocketSubStatusTableStatus(void* data, int dataSize);

	//---------------------��Ϸ�߼�--------------------//
public:
	//����������
	virtual bool SendHeartPak();
	//���󷿼�����
	virtual bool SendGameRoomInfo();
	//����Ѻע
	virtual bool SendJetInfo(word index, longlong jet);
protected:
	//����������
	virtual bool onSocketSubHeartPak(void* data, int dataSize);
	//�õ�SLWH��Ϸ������Ϣ
	virtual bool onSocketSubTableInfo(void* data, int dataSize);
	//������Ϣ������ͨ D3X  D4X��
	virtual bool onSocketSubOpenRlt(void* data, int dataSize);
	//�ʽ𿪽���Ϣ
	virtual bool onSocketSubOpenCJRlt(void* data, int dataSize);
	//���˳齱
	virtual bool onSocketSubOpenXYCJRlt(void* data, int dataSize);
	//�����Ὺ����Ϣ
	virtual bool onSocketSubOpenDMGRlt(void* data, int dataSize);
	//4����
	virtual bool onSocketSubOpen4LFRlt(void* data, int dataSize);
	//��ʼ��Ϸ
	virtual bool onSocketSubStartGame(void* data, int dataSize);
	//����Ҽ�����뿪
	virtual bool onSocketSubJoinOrExit(void* data, int dataSize);
	//���ѹ����Ϣͬ��֪ͨ
	virtual bool onSocketSubOtherUserJet(void* data, int dataSize);
	//֪ͨˢ������б�
	virtual bool onSocketSubUpdateUserList(void* data, int dataSize);
	//�����ȡ����ѹ��
	virtual bool onSocketSubOtherCancelJet(void* data, int dataSize);

public:
	static NetworkKernel* getInstance();
protected:
	static NetworkKernel* m_instance;
};

#endif 