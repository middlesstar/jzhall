#ifndef  _WindowTicketSend_H_
#define  _WindowTicketSend_H_

#include "Window/WindowBase.h"

class  WindowTicketSend : public WindowBase
{
public:
	WindowTicketSend();
	virtual ~WindowTicketSend();

	CREATE_FUNC(WindowTicketSend);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;

protected:
	void _addOneUnit();
	void _subOneUnit();

protected:
	Text* m_pTextSendNum;
	TextField* m_pIDInput;

	int m_nSendNum;			//赠送船票的数量
};

#endif // _WindowTicketSend_H_

