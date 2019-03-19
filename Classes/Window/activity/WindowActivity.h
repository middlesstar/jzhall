#ifndef  _WindowActivity_H_
#define  _WindowActivity_H_

#include "Window/WindowBase.h"

/* �����*/
class WindowActivity : public WindowBase
{
public:
	WindowActivity();
	virtual ~WindowActivity();

	CREATE_FUNC(WindowActivity);
	virtual void setContent();				                //����UI����
	virtual void onEnter() override;
	virtual void updateWindow(int nCode, void* pData) override;	
	virtual void updateWindow(int nCode) override;	        //ˢ�´���
	void StartTimeCount(int t);		
protected:
	
	//ʱ�䵹��ʱ
	void updateTime(float t);
private:
	

};

#endif // _WindowActivity_H_

