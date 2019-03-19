#ifndef  _WindowSetting_H_
#define  _WindowSetting_H_

#include "Window/WindowBase.h"

/* 窗口基类 */
class  WindowSetting : public WindowBase
{
public:
	WindowSetting();
	virtual ~WindowSetting();

	CREATE_FUNC(WindowSetting);
	virtual void setContent();				                //设置UI布局
	virtual void onEnter() override;

protected:
	void _onEffectOnOffClick(Ref* pRef);
	void _onSoundOnOffClick(Ref* pRef);

protected:
	ImageView* m_pEffectOnOff;			//音效开关
	ImageView* m_pSoundOnOff;			//背景音乐开关
};

#endif // _WindowSetting_H_

