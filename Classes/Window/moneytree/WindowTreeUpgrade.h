#ifndef  _WindowTreeUpgrade_H_
#define  _WindowTreeUpgrade_H_

#include "Window/WindowBase.h"

class  WindowTreeUpgrade : public WindowBase
{
public:
	WindowTreeUpgrade();
	virtual ~WindowTreeUpgrade();

	CREATE_FUNC(WindowTreeUpgrade);

	virtual void setContent();				                //…Ë÷√UI≤ºæ÷
	virtual void onEnter() override;
};

#endif // _WindowTreeUpgrade_H_

