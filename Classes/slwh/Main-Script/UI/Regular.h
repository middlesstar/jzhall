#ifndef __Regular_H__
#define __Regular_H__

#include "slwh/Base/UIBase.h"

/*
���й��������UILayerMain ���ʼ��
������UILayerMain���� ���õ���ģʽ
*/

class Regular :
	public UIBase
{
protected:
	Button*						m_close;
	LayerColor*					m_layerColor;

	bool						m_bIsInit;
protected:
	bool						m_bIsCloseRunning;
	//����
	Action*						m_fadein;
	Action*						m_fadeout;

protected:
	static Regular*				m_instance;

public:
	~Regular();

	void Show();
	void Hide();


protected:
	//��ֹ�������
	Regular();
	void Start();

	void CloseEvent(Ref* pSender, TouchEventType type);

	//����رհ�ť�󣬶���������ɵĻص�
	void CallBackClose();

	bool onTouchBegan(Touch* touch, Event* event);

	CREATE_FUNC(Regular)
public:
	static Regular* GetInstance();
};

#endif

