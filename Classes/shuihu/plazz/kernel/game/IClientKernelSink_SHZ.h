#ifndef SHZ_IClientKernelSink_H_
#define SHZ_IClientKernelSink_H_

#include "shuihu/plazz/kernel/user/IClientUserItem_SHZ.h"

class SHZIClientKernelSink
{
public:
	virtual ~SHZIClientKernelSink(){};

	//���ƽӿ�
public:
	//������Ϸ
	virtual bool SetupGameClient()=0;
	//������Ϸ
	virtual void ResetGameClient()=0;
	//�ر���Ϸ
	virtual void CloseGameClient()=0;

	//����¼�
public:
	//ϵͳ������Ϣ
	virtual bool OnGFTableMessage(const char* szMessage)=0;
	//�ȴ���ʾ
	virtual bool OnGFWaitTips(bool bWait)=0;
	//������Ϣ
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo)=0;
	//�����ȴ���ʾ
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip)=0;
	//�������
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult)=0;

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(void* data, int dataSize)=0;
	//������Ϣ
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)=0;
	//������Ϣ
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize)=0;

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//�û��뿪
	virtual void OnEventUserLeave(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//�û�����
	virtual void OnEventUserScore(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//�û�״̬
	virtual void OnEventUserStatus(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//�û�����
	virtual void OnEventUserAttrib(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
	//�û�ͷ��
	virtual void OnEventCustomFace(SHZIClientUserItem * pIClientUserItem, bool bLookonUser)=0;
};

#endif // _IClientKernelSink_H_