#ifndef __WaitNext_H__
#define __WaitNext_H__

#include "shark/Base/ViewBase.h"

/*
������Ϸ�ȴ�����
*/


class WaitNext :
	public ViewBase
{
protected:
	TextAtlas*			m_timeCount;
	ImageView*			m_waitnextgame;
	Button*				m_okbt;
	Node*				m_waitnode;
	int					m_nowTime;					//��ǰʣ���ʱ��
protected:
	void setContent();

	void BtEvent(Ref* pSender, TouchEventType type);

	void updatetime(float t);
	//��ʱ��3��ر�֪ͨ��
	void updateNoticeOut(float t);
	//���ֶ���
	void updateText(float t);
	//�ȴ�֪ͨ���˳�
	void waitnodeOutFunc();
	
public:
	WaitNext();

	virtual ~WaitNext();

	void StartPlay(int time);
	//����(����ʹ��)
	void destory();
};

#endif

