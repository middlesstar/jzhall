#ifndef __UIMenu_H__
#define __UIMenu_H__

#include "slwh/Base/UIBase.h"
#include "Regular.h"

/*
���Ͻ�3����ť����

*/


class UIMenu :
	public UIBase
{

protected:
	Button*				m_recharge;		//��ֵ��ť
	Button*				m_setting;		//���ð�ť
	Button*				m_regular;		//����ť
	Button*				m_exit;			//�˳�

public:

	UIMenu();
	~UIMenu();

	void exit(int code = 0);	//0�˳����䵽ѡ������棬1�˳����䵽����

protected:
	void BtEvent(Ref *pSender, TouchEventType type);
	void Start();

};

#endif

