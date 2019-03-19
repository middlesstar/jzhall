#ifndef  _WindowFMDialyPrize_H_
#define  _WindowFMDialyPrize_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowFMDialyPrize : public WindowBase
{
public:
	WindowFMDialyPrize();
	virtual ~WindowFMDialyPrize();

	CREATE_FUNC(WindowFMDialyPrize);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void updateWindow(int nCode) override;	        //刷新窗口
protected:
	void _startTurntable(int nStopPos, const std::function<void()>& onTurntableStop);		//nStopPos从索引0开始
	void _stopCallback();
protected:
	ImageView* m_pTurntable;
	float m_fStartRorate;
};

#endif // _WindowFMDialyPrize_H_

