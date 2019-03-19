#ifndef  _WindowFMComment_H_
#define  _WindowFMComment_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowFMComment : public WindowBase
{
public:
	WindowFMComment();
	virtual ~WindowFMComment();

	CREATE_FUNC(WindowFMComment);
	virtual void setContent();				                //设置UI布局

	virtual void onEnter() override;
};

#endif // _WindowFMComment_H_

