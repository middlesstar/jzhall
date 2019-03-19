#ifndef  _WindowFMDole_H_
#define  _WindowFMDole_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowFMDole : public WindowBase
{
public:
	WindowFMDole();
	virtual ~WindowFMDole();

	CREATE_FUNC(WindowFMDole);
	virtual void setContent();				                //设置UI布局

	virtual void onEnter() override;
};

#endif // _WindowFMDole_H_

