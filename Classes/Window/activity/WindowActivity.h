#ifndef  _WindowActivity_H_
#define  _WindowActivity_H_

#include "Window/WindowBase.h"

/* 活动窗口*/
class WindowActivity : public WindowBase
{
public:
	WindowActivity();
	virtual ~WindowActivity();

	CREATE_FUNC(WindowActivity);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;	
	virtual void updateWindow(int nCode) override;	        //刷新窗口
	void StartTimeCount(int t);		
protected:
	
	//时间倒计时
	void updateTime(float t);
private:
	

};

#endif // _WindowActivity_H_

