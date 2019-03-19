#ifndef SHZ_ServerScene_H_
#define SHZ_ServerScene_H_

#include "cocos2d.h"

class SHZChatLayer;


//��Ϸ����
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
	//��½��Ϣ
public:
	//����ʧ��
	virtual void onGRRequestFailure(const std::string& sDescribeString);
	//��½�ɹ�
	virtual void OnGRLogonSuccess();
	//��½ʧ��
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString);
	//��½���
	virtual void OnGRLogonFinish();
	//����֪ͨ
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString);
	void OnUpdateRequest(Node* node);

	//������Ϣ
public:
	//�б�����
	virtual void OnGRConfigColumn();
	//��������
	virtual void OnGRConfigServer();
	//��������
	virtual void OnGRConfigProperty();
	//���Ȩ������
	virtual void OnGRConfigUserRight() ;
	//�������
	virtual void OnGRConfigFinish();

	//�û���Ϣ
public:
	//�û�����
	virtual void OnGRUserEnter(SHZIClientUserItem* pIClientUserItem);
	//�û�����
	virtual void OnGRUserUpdate(SHZIClientUserItem* pIClientUserItem);
	//�û�ɾ��
	virtual void OnGRUserDelete(SHZIClientUserItem* pIClientUserItem);

	//�����Ϣ
public:
	//�û�����
	virtual void OnGFUserInvite(const std::string& szMessage);
	//�û�����ʧ��
	virtual void OnGFUserInviteFailure(const std::string& szMessage);
	//�����˳�
	virtual void OnGFServerClose(const std::string& szMessage);

	//������Ϸ�ں�
	virtual bool CreateKernel();

	//��������
public:
	void func_server_close(float dt);
private:
	HallGameServerItem*	mGameServerItem;
	SHZIServerItem*		mServerItem;
	SHZChatLayer*			mChatLayer;

	std::string			mMessageTemp;
};
#endif // _ServerScene_H_