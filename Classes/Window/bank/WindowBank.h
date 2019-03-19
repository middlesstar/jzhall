#ifndef  _WindowBank_H_
#define  _WindowBank_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowBank : public WindowBase
{
public:
	WindowBank();
	virtual ~WindowBank();

	CREATE_FUNC(WindowBank);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;

	void refreshCoin(Ref* ref);
protected:
	void _setData();
	void _setEvent();

protected:
	//TextField* m_pSaveInput;
	//TextField* m_pDrawInput;
	cocos2d::ui::EditBox* m_pSaveInput;
	cocos2d::ui::EditBox* m_pDrawInput;
};

#endif // _WindowBank_H_

