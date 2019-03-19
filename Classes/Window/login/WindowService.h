#ifndef  _WindowService_H_
#define  _WindowService_H_

#include "Window/WindowBase.h"

class  WindowService : public WindowBase
{
public:
	WindowService();
	virtual ~WindowService();

	CREATE_FUNC(WindowService);

	virtual void setContent() override;				        //…Ë÷√UI≤ºæ÷
};

#endif // _WindowService_H_

