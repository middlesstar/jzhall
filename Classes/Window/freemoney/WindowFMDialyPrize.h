#ifndef  _WindowFMDialyPrize_H_
#define  _WindowFMDialyPrize_H_

#include "Window/WindowBase.h"

/* ���ڻ��� */
class  WindowFMDialyPrize : public WindowBase
{
public:
	WindowFMDialyPrize();
	virtual ~WindowFMDialyPrize();

	CREATE_FUNC(WindowFMDialyPrize);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void updateWindow(int nCode) override;	        //ˢ�´���
protected:
	void _startTurntable(int nStopPos, const std::function<void()>& onTurntableStop);		//nStopPos������0��ʼ
	void _stopCallback();
protected:
	ImageView* m_pTurntable;
	float m_fStartRorate;
};

#endif // _WindowFMDialyPrize_H_

