#ifndef _RuleShow_H_
#define _RuleShow_H_

#include "cocos2d.h"
#include "Window/WindowBase.h"



class RuleShow : public WindowBase
{
public:
	RuleShow();
	~RuleShow();
	CREATE_FUNC(RuleShow);

	virtual void setContent();				                //…Ë÷√UI≤ºæ÷
	virtual void onEnter() override;

protected:
	Button*    closeBtn;

};


#endif // _RateShow_H_