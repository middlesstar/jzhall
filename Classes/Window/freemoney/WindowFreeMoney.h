#ifndef  _WindowFreeMoney_H_
#define  _WindowFreeMoney_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowFreeMoney : public WindowBase
{
public:
	WindowFreeMoney();
	virtual ~WindowFreeMoney();

	CREATE_FUNC(WindowFreeMoney);
	virtual void setContent();				                //设置UI布局

	virtual void onEnter() override;
};

#endif // _WindowFreeMoney_H_

