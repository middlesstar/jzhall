#ifndef  _WindowFMDole_H_
#define  _WindowFMDole_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowFMDole : public WindowBase
{
public:
	WindowFMDole();
	virtual ~WindowFMDole();

	CREATE_FUNC(WindowFMDole);
	virtual void setContent();				                //����UI����

	virtual void onEnter() override;
};

#endif // _WindowFMDole_H_

