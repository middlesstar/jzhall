#ifndef  _WindowSetting_H_
#define  _WindowSetting_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowSetting : public WindowBase
{
public:
	WindowSetting();
	virtual ~WindowSetting();

	CREATE_FUNC(WindowSetting);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;

protected:
	void _onEffectOnOffClick(Ref* pRef);
	void _onSoundOnOffClick(Ref* pRef);

protected:
	ImageView* m_pEffectOnOff;			//��Ч����
	ImageView* m_pSoundOnOff;			//�������ֿ���
};

#endif // _WindowSetting_H_

