#ifndef  _WindowShare_H_
#define  _WindowShare_H_

#include "Window/WindowBase.h"

class  WindowShare : public WindowBase
{
public:
	WindowShare();
	virtual ~WindowShare();

	CREATE_FUNC(WindowShare);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;

protected:
	void _shareWeiXin(Ref* sender);							//分享到微信
	void _shareWeiXinFriendsCicle(Ref* sender);				//分享到微信朋友圈
	void _shareQQZone(Ref* sender);							//分享到qq空间
};

#endif // _WindowShare_H_

