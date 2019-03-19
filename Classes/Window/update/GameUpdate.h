#ifndef  _GameUpdate_H_
#define  _GameUpdate_H_

#include "Window/WindowBase.h"

class  GameUpdate : public WindowBase
{
public:
	GameUpdate();
	virtual ~GameUpdate();

	CREATE_FUNC(GameUpdate);

	virtual void setContent() override;				        //…Ë÷√UI≤ºæ÷
};

#endif // _WindowService_H_

