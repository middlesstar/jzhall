#ifndef SHZ__ClientKernelSink_H_
#define SHZ__ClientKernelSink_H_

#include "cocos2d.h"
#include "GameClientView_SHZ.h"

class SHZClientKernelSink
	: public SHZIClientKernelSink
{

	//���ƽӿ�
public:

	//������Ϸ
	virtual bool SetupGameClient();
	//������Ϸ
	virtual void ResetGameClient();
	//�ر���Ϸ
	virtual void CloseGameClient();

	//����¼�
public:
	//ϵͳ������Ϣ
	virtual bool OnGFTableMessage(const char* szMessage);
	//�ȴ���ʾ
	virtual bool OnGFWaitTips(bool bWait);
	//������Ϣ++
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo);
	//�����ȴ���ʾ
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip);
	//�������
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult);

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û��뿪
	virtual void OnEventUserLeave(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�����
	virtual void OnEventUserScore(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�״̬
	virtual void OnEventUserStatus(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�����
	virtual void OnEventUserAttrib(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};
	//�û�ͷ��
	virtual void OnEventCustomFace(SHZIClientUserItem * pIClientUserItem, bool bLookonUser){};


	//////////////////////////////////////////////////////////////////////////

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(void* data, int dataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize);

	//��Ϣ����
protected:
	//��ʼ����
	virtual bool  StartGetType();
	//��ȡ�÷�
	virtual bool  GetWinScore(int gettype);
	//���·�
	virtual bool AddOrRemoveScore(bool addorremove);
	//������ע
	virtual bool  ChangeCellScore();
	//
	virtual bool MaLiSendMessage();

	//��ȡ�ȱ�����
	virtual bool  GetBiBeiData();





	//�л���λ
	int SwitchChairID(int m_Currchairid);
	
	//////////////////////////////////////////////////////////////////////////
	// ���ܺ���
public:
	void send_time_check();

	
	//////////////////////////////////////////////////////////////////////////
	

private:
	
	bool					mIsNetworkPrepared;
	SHZCGameClientView*		m_GameClientView;					//��Ϸ��ͼ

};

extern SHZClientKernelSink gClientKernelSink_SHZ;
#endif // _ClientKernelSink_H_