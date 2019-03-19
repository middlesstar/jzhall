#ifndef  _WindowVipGift_H_
#define  _WindowVipGift_H_

#include "Window/WindowBase.h"

/* VIP礼包界面 */
class  WindowVipGift : public WindowBase
{
public:
	WindowVipGift();
	virtual ~WindowVipGift();

	CREATE_FUNC(WindowVipGift);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
};

#endif // _WindowVipGift_H_

