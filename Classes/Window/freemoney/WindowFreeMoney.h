#ifndef  _WindowFreeMoney_H_
#define  _WindowFreeMoney_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowFreeMoney : public WindowBase
{
public:
	WindowFreeMoney();
	virtual ~WindowFreeMoney();

	CREATE_FUNC(WindowFreeMoney);
	virtual void setContent();				                //����UI����

	virtual void onEnter() override;
};

#endif // _WindowFreeMoney_H_

