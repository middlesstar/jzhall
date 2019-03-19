#ifndef  _WindowFMWeixin_H_
#define  _WindowFMWeixin_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowFMWeixin : public WindowBase
{
public:
	WindowFMWeixin();
	virtual ~WindowFMWeixin();

	CREATE_FUNC(WindowFMWeixin);
	virtual void setContent();				                //设置UI布局

	virtual void onEnter() override;

protected:
	TextField* m_pInput;
};

#endif // _WindowFMWeixin_H_

