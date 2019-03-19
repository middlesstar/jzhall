#ifndef  _WindowShare_H_
#define  _WindowShare_H_

#include "Window/WindowBase.h"

class  WindowShare : public WindowBase
{
public:
	WindowShare();
	virtual ~WindowShare();

	CREATE_FUNC(WindowShare);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;

protected:
	void _shareWeiXin(Ref* sender);							//����΢��
	void _shareWeiXinFriendsCicle(Ref* sender);				//����΢������Ȧ
	void _shareQQZone(Ref* sender);							//����qq�ռ�
};

#endif // _WindowShare_H_

