#ifndef  _WindowFMComment_H_
#define  _WindowFMComment_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowFMComment : public WindowBase
{
public:
	WindowFMComment();
	virtual ~WindowFMComment();

	CREATE_FUNC(WindowFMComment);
	virtual void setContent();				                //����UI����

	virtual void onEnter() override;
};

#endif // _WindowFMComment_H_

