#ifndef __WaitNextJet_H__
#define __WaitNextJet_H__

#include "slwh/Base/UIBase.h"

/*
������Ϸ�ȴ�����
*/


class WaitNextJet :
	public UIBase
{
protected:
	TextAtlas*			m_timeCount;
	ImageView*			m_waitnextgame;
	Button*				m_okbt;
	Node*				m_waitnode;
	int					m_nowTime;					//��ǰʣ���ʱ��
protected:
	void Start();

	void BtEvent(Ref* pSender, TouchEventType type);

	void updatetime(float t);
	//��ʱ��3��ر�֪ͨ��
	void updateNoticeOut(float t);
	//���ֶ���
	void updateText(float t);
	//�ȴ�֪ͨ���˳�
	void waitnodeOutFunc();
	
public:
	WaitNextJet();

	virtual ~WaitNextJet();

	void StartPlay(int time);
	//����(����ʹ��)
	void destory();
};

#endif

