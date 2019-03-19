#ifndef __SharkMenu_H__
#define __SharkMenu_H__

#include "shark/Base/ViewBase.h"
//#include "Regular.h"

/*
���Ͻ�3����ť����

*/


class SharkMenu :
	public ViewBase
{

protected:
	Node*               menu;           //�˵���ʾ
	Button*				m_recharge;		//��ֵ��ť
	Button*				m_setting;		//���ð�ť
	Button*				m_regular;		//����ť
	Button*				m_exit;			//�˳�
	Button*             m_left;         //�˵����ذ�ť
	Button*             m_right;        //�˵�������ť

	bool                flag;           //�˵����������ı�־

public:

	SharkMenu();
	~SharkMenu();
	//���ذ�ť�¼� 
	void goBack(int code = 0);

	//������ֵ��ť
	void outRecharge(Ref* ref);

protected:
	void BtEvent(Ref *pSender, TouchEventType type);
	void setContent();
	//�˵��ջ�
	void moveIn();
	//�˵���ʾ
	void moveOut();
	//���ð�ť�ɼ�
	void setRight();


};

#endif

