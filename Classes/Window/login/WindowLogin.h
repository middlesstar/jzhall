#ifndef  _WindowLogin_H_
#define  _WindowLogin_H_

#include "Window/WindowBase.h"

class  WindowLogin : public WindowBase
{
public:
	WindowLogin();
	virtual ~WindowLogin();

	CREATE_FUNC(WindowLogin);
	virtual void setContent();				                //����UI����

	virtual void updateWindow(int nCode) override;	        //ˢ�´���
protected:
	void _initLoginAccount();
	void _setLoginAccount(const std::string& strNickName);
	void _saveLoginAccount();
	
	void _addButtonLight();
};

#endif // _WindowLogin_H_

