#ifndef  _WindowVipGift_H_
#define  _WindowVipGift_H_

#include "Window/WindowBase.h"

/* VIP������� */
class  WindowVipGift : public WindowBase
{
public:
	WindowVipGift();
	virtual ~WindowVipGift();

	CREATE_FUNC(WindowVipGift);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;
};

#endif // _WindowVipGift_H_

