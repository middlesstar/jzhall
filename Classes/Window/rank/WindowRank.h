#ifndef  _WindowRank_H_
#define  _WindowRank_H_

#include "Window/WindowBase.h"

class  WindowRank : public WindowBase
{
public:
	WindowRank();
	virtual ~WindowRank();

	CREATE_FUNC(WindowRank);
	virtual void setContent() override;				                //����UI����
protected:

};

#endif // _WindowRank_H_

