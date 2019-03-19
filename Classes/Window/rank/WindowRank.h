#ifndef  _WindowRank_H_
#define  _WindowRank_H_

#include "Window/WindowBase.h"

class  WindowRank : public WindowBase
{
public:
	WindowRank();
	virtual ~WindowRank();

	CREATE_FUNC(WindowRank);
	virtual void setContent() override;				                //…Ë÷√UI≤ºæ÷
protected:

};

#endif // _WindowRank_H_

