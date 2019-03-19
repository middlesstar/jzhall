#ifndef  _WindowTicketSend_H_
#define  _WindowTicketSend_H_

#include "Window/WindowBase.h"

class  WindowTicketSend : public WindowBase
{
public:
	WindowTicketSend();
	virtual ~WindowTicketSend();

	CREATE_FUNC(WindowTicketSend);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;

protected:
	void _addOneUnit();
	void _subOneUnit();

protected:
	Text* m_pTextSendNum;
	TextField* m_pIDInput;

	int m_nSendNum;			//���ʹ�Ʊ������
};

#endif // _WindowTicketSend_H_

