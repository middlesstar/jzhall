#ifndef  _LogicExpedition_H_
#define  _LogicExpedition_H_

#include "Window/LogicBase.h"
class LogicExpedition : public LogicBase
{
public:
	LogicExpedition();
	virtual ~LogicExpedition();

	SINGLE_INSTANCE_FUNC(LogicExpedition);

protected:
	void _registerMessageHandler();

private:

};



#endif // _LogicExpedition_H_

